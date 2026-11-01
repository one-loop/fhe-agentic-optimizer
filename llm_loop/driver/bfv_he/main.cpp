// Runs one generated BFV program of the LLM loop with the output format of the generated
// CKKS main (gen_main_code_ckks): an exact check on every output, and fhe()'s median time.
// Parameters are fixed to those of the BFV records (N = 16384, BFVDefault, 20-bit plaintext
// modulus), so latencies of different kernels are comparable.
//
//   ./main [--tol T] [--repeats N]     T is accepted and ignored: BFV results are exact
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "_gen_he_fhe.hpp"
#include "utils.hpp"

using namespace std;
using namespace seal;

int main(int argc, char **argv)
{
  size_t repeats = 10;
  for (int i = 1; i < argc; ++i)
  {
    const string arg = argv[i];
    if (arg == "--tol" && i + 1 < argc)
      ++i;
    else if (arg == "--repeats" && i + 1 < argc)
      repeats = stoul(argv[++i]);
    else
    {
      cerr << "usage: " << argv[0] << " [--tol T] [--repeats N]\n";
      return 2;
    }
  }
  ifstream is("../fhe_io_example_adapted.txt");
  if (!is)
    throw invalid_argument("failed to open io example file");

  EncryptionParameters params(scheme_type::bfv);
  const size_t n = 16384;
  params.set_poly_modulus_degree(n);
  params.set_plain_modulus(PlainModulus::Batching(n, 20));
  params.set_coeff_modulus(CoeffModulus::BFVDefault(n));
  SEALContext context(params, true, sec_level_type::tc128);
  BatchEncoder batch_encoder(context);
  KeyGenerator keygen(context);
  const SecretKey &secret_key = keygen.secret_key();
  PublicKey public_key;
  keygen.create_public_key(public_key);
  RelinKeys relin_keys;
  keygen.create_relin_keys(relin_keys);
  GaloisKeys galois_keys;
  keygen.create_galois_keys(galois_keys);
  Encryptor encryptor(context, public_key);
  Evaluator evaluator(context);
  Decryptor decryptor(context, secret_key);

  ClearArgsInfo clear_inputs, clear_outputs;
  size_t func_slot_count;
  EncryptedArgs encrypted_inputs;
  EncodedArgs encoded_inputs;
  EncryptedArgs encrypted_outputs;
  EncodedArgs encoded_outputs;
  const auto t = chrono::high_resolution_clock::now();
  parse_inputs_outputs_file(is, params.plain_modulus().value(), clear_inputs, clear_outputs, func_slot_count);
  prepare_he_inputs(batch_encoder, encryptor, clear_inputs, encrypted_inputs, encoded_inputs);
  fhe(
    encrypted_inputs, encoded_inputs, encrypted_outputs, encoded_outputs, batch_encoder, encryptor, evaluator,
    relin_keys, galois_keys);
  const chrono::duration<double, milli> elapsed = chrono::high_resolution_clock::now() - t;

  // One warmup run and `repeats` timed runs of fhe() on the same encrypted inputs.
  vector<double> eval_ms;
  for (size_t i = 0; i <= repeats; ++i)
  {
    EncryptedArgs run_encrypted_outputs;
    EncodedArgs run_encoded_outputs;
    const auto start = chrono::high_resolution_clock::now();
    fhe(
      encrypted_inputs, encoded_inputs, run_encrypted_outputs, run_encoded_outputs, batch_encoder, encryptor,
      evaluator, relin_keys, galois_keys);
    const chrono::duration<double, milli> run = chrono::high_resolution_clock::now() - start;
    if (i > 0)
      eval_ms.push_back(run.count());
  }

  ClearArgsInfo obtained;
  get_clear_outputs(batch_encoder, decryptor, encrypted_outputs, encoded_outputs, func_slot_count, obtained);
  const uint64_t t_mod = params.plain_modulus().value();
  bool ok = true;
  for (const auto &[label, expected] : clear_outputs)
  {
    const auto it = obtained.find(label);
    if (it == obtained.end())
    {
      cout << "output " << label << ": max_abs_error=inf (missing)\n";
      ok = false;
      continue;
    }
    const auto &want = get<vector<uint64_t>>(expected.value_);
    const auto &got = get<vector<uint64_t>>(it->second.value_);
    uint64_t error = 0;
    for (size_t i = 0; i < want.size(); ++i)
    {
      const uint64_t diff = want[i] > got[i] ? want[i] - got[i] : got[i] - want[i];
      error = max(error, min(diff, t_mod - diff));  // distance modulo the plaintext modulus
    }
    const auto budget = encrypted_outputs.count(label) ? decryptor.invariant_noise_budget(encrypted_outputs.at(label)) : -1;
    cout << "output " << label << ": max_abs_error=" << error << (error == 0 ? " (ok)" : " (not exact)")
         << ", noise budget " << budget << " bits\n";
    ok = ok && error == 0;
  }
  cout << "execution_time_(ms): " << elapsed.count() << "\n";
  if (!eval_ms.empty())
  {
    sort(eval_ms.begin(), eval_ms.end());
    const size_t m = eval_ms.size() / 2;
    const double median = eval_ms.size() % 2 ? eval_ms[m] : (eval_ms[m - 1] + eval_ms[m]) / 2;
    cout << "fhe_eval_median_(ms): " << median << " (1 warmup + " << repeats << " runs)\n";
  }
  cout << (ok ? "PASS" : "FAIL") << " (exact)\n";
  return ok ? 0 : 1;
}
