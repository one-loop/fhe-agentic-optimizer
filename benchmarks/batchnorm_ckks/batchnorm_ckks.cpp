#include "fheco/fheco.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;
using namespace fheco;

// Inference-time BatchNorm in the CHEHAB DSL, compiled for CKKS. The
// statistics and affine parameters are folded offline into per-slot
// A = gamma / sqrt(running_var + eps) and B = beta - A * running_mean, packed
// for the NCL (BatchNorm1d) or NCHW (BatchNorm2d) layout by
// generate_batchnorm_ckks; the encrypted arithmetic is the same for both.
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}

int main(int argc, char **argv)
{
  // ./batchnorm_ckks [slot_count] [call_quantifier] [cse] [const_folding]
  size_t slot_count = 8;
  if (argc > 1)
    slot_count = stoul(argv[1]);

  bool call_quantifier = true;
  if (argc > 2)
    call_quantifier = stoi(argv[2]);

  bool cse = true;
  if (argc > 3)
    cse = stoi(argv[3]);

  bool const_folding = true;
  if (argc > 4)
    const_folding = stoi(argv[4]);

  if (cse)
  {
    Compiler::enable_cse();
    Compiler::enable_order_operands();
  }
  else
  {
    Compiler::disable_cse();
    Compiler::disable_order_operands();
  }

  if (const_folding)
    Compiler::enable_const_folding();
  else
    Compiler::disable_const_folding();

  // Same CKKS parameters as the direct-SEAL reference in
  // benchmarks/dl_operators_ckks: N = 32768, {60, 40 x 10, 60}, scale 2^40.
  CkksParams params{32768, {60, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 60}, 40};

  chrono::high_resolution_clock::time_point t;
  chrono::duration<double, milli> elapsed;
  string func_name = "fhe";
  t = chrono::high_resolution_clock::now();
  const auto &func = Compiler::create_ckks_func(func_name, slot_count, params);
  util::copyFile("fhe_io_example.txt", "fhe_io_example_adapted.txt");
  fhe();
  string gen_name = "_gen_he_" + func_name;
  string gen_path = "he/" + gen_name;
  ofstream header_os(gen_path + ".hpp");
  if (!header_os)
    throw logic_error("failed to create header file");
  ofstream source_os(gen_path + ".cpp");
  if (!source_os)
    throw logic_error("failed to create source file");
  Compiler::compile(func, Compiler::Ruleset::simplification_ruleset, trs::RewriteHeuristic::bottom_up);
  Compiler::gen_he_code(func, header_os, gen_name + ".hpp", source_os);
  elapsed = chrono::high_resolution_clock::now() - t;
  cout << "Compile time : \n";
  cout << elapsed.count() << " ms\n";
  if (call_quantifier)
  {
    util::Quantifier quantifier{func};
    quantifier.run_all_analysis();
    quantifier.print_info(cout);
  }
  return 0;
}
