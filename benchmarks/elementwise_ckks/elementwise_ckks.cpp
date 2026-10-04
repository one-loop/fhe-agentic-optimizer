#include "fheco/fheco.hpp"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
using namespace fheco;

// Elementwise CKKS add and multiply in the CHEHAB DSL, on an encrypted x0 and
// a second operand that is encrypted (x1), a plaintext slot vector (p) or a
// plaintext scalar (s, one value in the io file, encoded as a constant in every
// slot). Add costs no level (e.g. a ResNet residual join); multiply costs one
// (ciphertext-ciphertext: multiply, relinearize, rescale; ciphertext-plaintext:
// multiply_plain, rescale).
//   add         y = x0 + x1        mul         y = x0 * x1
//   add_plain   y = x0 + p         mul_plain   y = x0 * p
//   add_scalar  y = x0 + s         mul_scalar  y = x0 * s
// The plain and scalar programs are the same DSL; whether the operand is a
// vector or a scalar is decided by its io-file value count.
const vector<string> ops{"add", "add_plain", "add_scalar", "mul", "mul_plain", "mul_scalar"};

void fhe(const string &op)
{
  Ciphertext x0("x0");
  const bool is_add = op.rfind("add", 0) == 0;
  Ciphertext y;
  if (op == "add" || op == "mul")
  {
    Ciphertext x1("x1");
    y = is_add ? x0 + x1 : x0 * x1;
  }
  else
  {
    Plaintext operand(op.find("scalar") != string::npos ? "s" : "p");
    y = is_add ? x0 + operand : x0 * operand;
  }
  y.set_output("y");
}

int main(int argc, char **argv)
{
  // ./elementwise_ckks [op] [slot_count] [call_quantifier] [cse] [const_folding]
  string op = "add";
  if (argc > 1)
    op = argv[1];
  if (find(ops.begin(), ops.end(), op) == ops.end())
    throw invalid_argument("op must be add, add_plain, add_scalar, mul, mul_plain or mul_scalar");

  size_t slot_count = 8;
  if (argc > 2)
    slot_count = stoul(argv[2]);

  bool call_quantifier = true;
  if (argc > 3)
    call_quantifier = stoi(argv[3]);

  bool cse = true;
  if (argc > 4)
    cse = stoi(argv[4]);

  bool const_folding = true;
  if (argc > 5)
    const_folding = stoi(argv[5]);

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
  fhe(op);
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
