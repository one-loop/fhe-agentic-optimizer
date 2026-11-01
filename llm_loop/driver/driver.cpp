// Compiles one kernel through CHEHAB exactly like benchmarks/*_ckks (or, with
// bfv, the scalar path of benchmarks/nn_*): create_ckks_func (create_func), the
// kernel's fhe(), Compiler::compile (simplification ruleset, bottom-up),
// Compiler::gen_he_code, then the Quantifier report.
//
//   ./llm_driver <slot_count> [ckks|bfv]
//
// Run it in a directory holding fhe_io_example.txt and an he/ subdirectory;
// it writes he/_gen_he_fhe.{hpp,cpp} and he/main.cpp. The kernel (kernel.inc)
// defines `void fhe()` and may call slots(), the function's slot count.
#include "fheco/fheco.hpp"

#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
using namespace fheco;

namespace llm_loop
{
size_t slot_count = 0;
} // namespace llm_loop

inline size_t slots()
{
  return llm_loop::slot_count;
}

#include "kernel.inc"

int main(int argc, char **argv)
{
  if (argc != 2 && argc != 3)
  {
    cerr << "usage: " << argv[0] << " <slot_count> [ckks|bfv]\n";
    return 2;
  }
  llm_loop::slot_count = stoul(argv[1]);
  const bool bfv = argc == 3 && string(argv[2]) == "bfv";

  // Settings of run_benchmarks.py's default CKKS runs (cse = 1, const_folding = 1).
  Compiler::enable_cse();
  Compiler::enable_order_operands();
  Compiler::enable_const_folding();

  // Parameters shared by every CKKS benchmark: N = 32768, {60, 40 x 10, 60}, scale 2^40.
  CkksParams params{32768, {60, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 60}, 40};

  const auto start = chrono::high_resolution_clock::now();
  // BFV: the benchmarks' 20-bit plaintext modulus, signed values. slots() is the operator's width;
  // CHEHAB needs a power of two here, and nothing in the scalar path's generated code depends on it.
  size_t bfv_slots = 1;
  while (bfv && bfv_slots < llm_loop::slot_count)
    bfv_slots *= 2;
  const auto &func = bfv ? Compiler::create_func("fhe", bfv_slots, 20, false, true)
                         : Compiler::create_ckks_func("fhe", llm_loop::slot_count, params);
  util::copyFile("fhe_io_example.txt", "fhe_io_example_adapted.txt");
  fhe();
  ofstream header_os("he/_gen_he_fhe.hpp");
  ofstream source_os("he/_gen_he_fhe.cpp");
  if (!header_os || !source_os)
    throw logic_error("failed to create he/_gen_he_fhe.*");
  Compiler::compile(func, Compiler::Ruleset::simplification_ruleset, trs::RewriteHeuristic::bottom_up);
  Compiler::gen_he_code(func, header_os, "_gen_he_fhe.hpp", source_os);
  const chrono::duration<double, milli> elapsed = chrono::high_resolution_clock::now() - start;
  cout << "Compile time : \n" << elapsed.count() << " ms\n";
  util::Quantifier quantifier{func};
  quantifier.run_all_analysis();
  quantifier.print_info(cout);
  return 0;
}
