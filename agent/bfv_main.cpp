// Runtime for CHEHAB-generated BFV programs evaluated by the optimization loop.
//
// The generated he/main.cpp of the BFV benchmarks runs fhe() once and prints
// one execution time, and the repository's golden test freezes that generated
// code byte for byte. The loop needs the same interface as the CKKS runtime
// (benchmarks/ckks_utils.cpp): a correctness verdict, one warmup plus
// `--repeats` timed runs of fhe() and their median. This main provides it,
// with the encryption parameters of the generated main (BFVDefault(16384),
// 20-bit batching plaintext modulus) and the repository's untouched
// benchmarks/utils.cpp for parsing, encryption, decryption and the exact
// comparison with the expected outputs.
//
//   ./main [--tol T] [--repeats N]     (--tol is accepted for interface parity; BFV is exact)
//
// Output lines read by agent/evaluate.py:
//   outputs_match: 1|0 (...)           from utils.cpp's get_clear_outputs
//   noise_budget_bits: B               smallest remaining invariant noise budget over outputs
//   execution_time_(ms): T             parse + encode + encrypt + first evaluation (CHEHAB convention)
//   fhe_eval_median_(ms): M (...)      fhe() alone, median of the timed runs
//   PASS | FAIL
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "_gen_he_fhe.hpp"
#include "utils.hpp"

using namespace std;
using namespace seal;

namespace
{
double median(vector<double> values)
{
  sort(values.begin(), values.end());
  const size_t n = values.size();
  return n % 2 ? values[n / 2] : (values[n / 2 - 1] + values[n / 2]) / 2;
}
} // namespace

int main(int argc, char **argv)
{
  size_t repeats = 10;
  for (int i = 1; i < argc; ++i)
  {
    string arg = argv[i];
    if (arg == "--repeats" && i + 1 < argc)
      repeats = stoul(argv[++i]);
    else if (arg == "--tol" && i + 1 < argc)
      ++i; // BFV results are exact integers; the tolerance does not apply
    else
    {
      cerr << "usage: " << argv[0] << " [--tol T] [--repeats N]\n";
      return 2;
    }
  }

  ifstream is("../fhe_io_example_adapted.txt");
  if (!is)
    throw invalid_argument("failed to open io example file");

  // Same parameters as the generated main of the BFV benchmarks.
  EncryptionParameters params(scheme_type::bfv);
  size_t n = 16384;
  params.set_poly_modulus_degree(n);
  params.set_plain_modulus(PlainModulus::Batching(n, 20));
  params.set_coeff_modulus(CoeffModulus::BFVDefault(16384));
  SEALContext context(params, true, sec_level_type::tc128);

  ClearArgsInfo clear_inputs, clear_outputs;
  size_t func_slot_count;
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
  EncryptedArgs encrypted_inputs;
  EncodedArgs encoded_inputs;

  // CHEHAB timing convention: parse the inputs, encode and encrypt them, evaluate once.
  auto t = chrono::high_resolution_clock::now();
  parse_inputs_outputs_file(is, params.plain_modulus().value(), clear_inputs, clear_outputs, func_slot_count);
  prepare_he_inputs(batch_encoder, encryptor, clear_inputs, encrypted_inputs, encoded_inputs);
  EncryptedArgs encrypted_outputs;
  EncodedArgs encoded_outputs;
  fhe(encrypted_inputs, encoded_inputs, encrypted_outputs, encoded_outputs, batch_encoder, encryptor, evaluator,
      relin_keys, galois_keys);
  chrono::duration<double, milli> elapsed = chrono::high_resolution_clock::now() - t;

  // Evaluation-only timing: the warmup above, then `repeats` timed runs on the same inputs.
  vector<double> eval_ms;
  for (size_t i = 0; i < repeats; ++i)
  {
    EncryptedArgs outs;
    EncodedArgs encs;
    auto t0 = chrono::high_resolution_clock::now();
    fhe(encrypted_inputs, encoded_inputs, outs, encs, batch_encoder, encryptor, evaluator, relin_keys,
        galois_keys);
    chrono::duration<double, milli> d = chrono::high_resolution_clock::now() - t0;
    eval_ms.push_back(d.count());
  }

  ClearArgsInfo obtained;
  get_clear_outputs(batch_encoder, decryptor, encrypted_outputs, encoded_outputs, func_slot_count, obtained);

  // Exact comparison, independent of the one utils.cpp prints.
  size_t mismatches = 0;
  for (const auto &[label, expected] : clear_outputs)
  {
    auto it = obtained.find(label);
    if (it == obtained.end() || !(it->second == expected))
      ++mismatches;
  }
  int noise = -1;
  for (const auto &[label, ct] : encrypted_outputs)
  {
    int b = decryptor.invariant_noise_budget(ct);
    noise = noise < 0 ? b : min(noise, b);
  }
  const bool ok = mismatches == 0 && !clear_outputs.empty() && noise > 0;

  cout << "outputs_checked: " << clear_outputs.size() - mismatches << "/" << clear_outputs.size()
       << " outputs equal the expected values (exact)\n";
  cout << "noise_budget_bits: " << noise << "\n";
  cout << "execution_time_(ms): " << elapsed.count() << "\n";
  if (!eval_ms.empty())
    cout << "fhe_eval_median_(ms): " << median(eval_ms) << " (1 warmup + " << repeats << " runs)\n";
  cout << (ok ? "PASS" : "FAIL") << " (exact integer comparison)\n";
  return ok ? 0 : 1;
}
