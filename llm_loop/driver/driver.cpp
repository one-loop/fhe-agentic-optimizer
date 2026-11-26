// Compiles one kernel through CHEHAB exactly like benchmarks/*_ckks (or, with
// bfv, the scalar path of benchmarks/nn_*): create_ckks_func (create_func), the
// kernel's fhe(), Compiler::compile (simplification ruleset, bottom-up),
// Compiler::gen_he_code, then the Quantifier report.
//
//   ./llm_driver <slot_count> [ckks|bfv] [fixed|sized] [simplification|depth|ops_cost|joined|none]
//
// Compiler mode (default simplification, the benchmarks' setting): which CHEHAB rewrite ruleset runs.
// `none` is the unoptimized reference: no term rewriting, and CSE, operand ordering and constant
// folding off, so the generated SEAL code follows the kernel's operations one to one.
//
// CKKS chain: `fixed` (default) always uses {60, 40 x 10, 60}; `sized` keeps only
// as many 40-bit primes as the compiled kernel consumes levels (plus the first and
// special primes), so neither the baseline nor a kernel pays for unused levels.
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
  const string mode = argc >= 5 ? argv[4] : "simplification";
  const bool known_mode = mode == "simplification" || mode == "depth" || mode == "ops_cost" || mode == "joined" ||
                          mode == "none";
  if (argc < 2 || argc > 5 || (argc >= 4 && string(argv[3]) != "fixed" && string(argv[3]) != "sized") || !known_mode)
  {
    cerr << "usage: " << argv[0] << " <slot_count> [ckks|bfv] [fixed|sized] [simplification|depth|ops_cost|joined|none]\n";
    return 2;
  }
  llm_loop::slot_count = stoul(argv[1]);
  const bool bfv = argc >= 3 && string(argv[2]) == "bfv";
  const bool sized_chain = argc >= 4 && string(argv[3]) == "sized";

  if (mode == "none")
  {
    Compiler::disable_cse();
    Compiler::disable_order_operands();
    Compiler::disable_const_folding();
  }
  else
  {
    // Settings of run_benchmarks.py's default CKKS runs (cse = 1, const_folding = 1).
    Compiler::enable_cse();
    Compiler::enable_order_operands();
    Compiler::enable_const_folding();
  }

  // Parameters shared by every CKKS benchmark: N = 32768, {60, 40 x 10, 60}, scale 2^40.
  CkksParams params{32768, {60, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 60}, 40};
  params.trim_to_levels_used = sized_chain;

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
  if (mode == "simplification")
    Compiler::compile(func, Compiler::Ruleset::simplification_ruleset, trs::RewriteHeuristic::bottom_up);
  else if (mode == "depth")
    Compiler::compile(func, Compiler::Ruleset::depth, trs::RewriteHeuristic::bottom_up);
  else if (mode == "ops_cost")
    Compiler::compile(func, Compiler::Ruleset::ops_cost, trs::RewriteHeuristic::bottom_up);
  else if (mode == "joined")
    Compiler::compile(func, Compiler::Ruleset::joined, trs::RewriteHeuristic::bottom_up);
  Compiler::gen_he_code(func, header_os, "_gen_he_fhe.hpp", source_os);
  const chrono::duration<double, milli> elapsed = chrono::high_resolution_clock::now() - start;
  cout << "Compile time : \n" << elapsed.count() << " ms\n";
  util::Quantifier quantifier{func};
  quantifier.run_all_analysis();
  quantifier.print_info(cout);
  return 0;
}
