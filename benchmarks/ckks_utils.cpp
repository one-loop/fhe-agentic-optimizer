#include "ckks_utils.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>

using namespace std;
using namespace seal;

namespace
{
istringstream next_line(istream &is, const char *what)
{
  string line;
  if (!getline(is, line))
    throw invalid_argument(string("io file: missing ") + what);
  return istringstream(line);
}

vector<double> read_values(istringstream &tokens, size_t count, const string &label)
{
  vector<double> values(count);
  for (auto &value : values)
  {
    if (!(tokens >> value))
      throw invalid_argument("io file: not enough values for " + label);
  }
  return values;
}

size_t chain_index(const SEALContext &context, const Ciphertext &ct)
{
  return context.get_context_data(ct.parms_id())->chain_index();
}
} // namespace

CkksIoExample parse_ckks_io_file(istream &is)
{
  CkksIoExample io;
  size_t nb_inputs, nb_outputs;
  auto header = next_line(is, "header");
  if (!(header >> io.func_slot_count >> nb_inputs >> nb_outputs))
    throw invalid_argument("io file: malformatted header");

  for (size_t i = 0; i < nb_inputs; ++i)
  {
    auto tokens = next_line(is, "input line");
    string label;
    int is_cipher, is_signed;
    if (!(tokens >> label >> is_cipher >> is_signed))
      throw invalid_argument("io file: malformatted input line");

    auto values = read_values(tokens, io.func_slot_count, label);
    (is_cipher ? io.cipher_inputs : io.plain_inputs).emplace(std::move(label), std::move(values));
  }

  for (size_t i = 0; i < nb_outputs; ++i)
  {
    auto tokens = next_line(is, "output line");
    string label;
    int is_cipher;
    if (!(tokens >> label >> is_cipher))
      throw invalid_argument("io file: malformatted output line");

    io.outputs.emplace(label, read_values(tokens, io.func_slot_count, label));
  }
  return io;
}

void prepare_ckks_inputs(
  const CKKSEncoder &encoder, const Encryptor &encryptor, double scale, const CkksIoExample &io,
  EncryptedArgs &encrypted_inputs, CkksClearArgs &plain_inputs)
{
  const size_t slot_count = encoder.slot_count();
  auto fill_slots = [slot_count](const vector<double> &values) {
    if (values.empty() || values.size() > slot_count)
      throw logic_error("input size must be in [1, slot_count]");

    vector<double> slots(slot_count);
    for (size_t i = 0; i < slot_count; ++i)
      slots[i] = values[i % values.size()];
    return slots;
  };

  for (const auto &[label, values] : io.cipher_inputs)
  {
    Plaintext encoded;
    encoder.encode(fill_slots(values), scale, encoded);
    Ciphertext encrypted;
    encryptor.encrypt(encoded, encrypted);
    encrypted_inputs.emplace(label, std::move(encrypted));
  }

  for (const auto &[label, values] : io.plain_inputs)
    plain_inputs.emplace(label, fill_slots(values));
}

CkksClearArgs decrypt_ckks_outputs(
  const CKKSEncoder &encoder, Decryptor &decryptor, const EncryptedArgs &encrypted_outputs, size_t func_slot_count)
{
  CkksClearArgs outputs;
  for (const auto &[label, encrypted] : encrypted_outputs)
  {
    Plaintext decrypted;
    decryptor.decrypt(encrypted, decrypted);
    vector<double> values;
    encoder.decode(decrypted, values);
    values.resize(func_slot_count);
    outputs.emplace(label, std::move(values));
  }
  return outputs;
}

bool check_ckks_outputs(
  const SEALContext &context, const EncryptedArgs &encrypted_inputs, const EncryptedArgs &encrypted_outputs,
  const CkksClearArgs &expected, const CkksClearArgs &obtained, double tolerance, ostream &os)
{
  bool ok = true;
  const bool have_input = !encrypted_inputs.empty();
  const size_t input_chain_index = have_input ? chain_index(context, encrypted_inputs.begin()->second) : 0;
  for (const auto &[label, want] : expected)
  {
    auto got_it = obtained.find(label);
    auto ct_it = encrypted_outputs.find(label);
    if (got_it == obtained.end() || ct_it == encrypted_outputs.end())
    {
      os << "output " << label << ": missing\n";
      ok = false;
      continue;
    }

    const auto &got = got_it->second;
    double max_error = 0;
    for (size_t i = 0; i < want.size(); ++i)
      max_error = max(max_error, abs(got[i] - want[i]));
    const bool pass = max_error <= tolerance;
    ok = ok && pass;

    const size_t output_chain_index = chain_index(context, ct_it->second);
    os << "output " << label << ": max_abs_error=" << max_error << (pass ? " (ok)" : " (exceeds tolerance)");
    if (have_input)
      os << ", chain_index " << input_chain_index << " -> " << output_chain_index << " (levels consumed "
         << input_chain_index - output_chain_index << ")";
    const auto precision = os.precision(9);
    os << ", log2_scale=" << log2(ct_it->second.scale()) << '\n';
    os.precision(precision);
  }
  return ok;
}

double median(vector<double> values)
{
  if (values.empty())
    throw invalid_argument("median of an empty vector");

  sort(values.begin(), values.end());
  const size_t n = values.size();
  return n % 2 == 1 ? values[n / 2] : 0.5 * (values[n / 2 - 1] + values[n / 2]);
}
