// Compiles one candidate CHEHAB DSL kernel for CKKS, exactly like the
// benchmarks under benchmarks/*_ckks: Compiler::compile (simplification
// ruleset, bottom-up) then Compiler::gen_he_code, then the Quantifier report.
//
//   ./agent_kernel <slot_count>     (run inside a directory with fhe_io_example.txt and he/)
//
// The kernel (kernel.inc, written by agent/loop.py) defines `void fhe()`.
#include "fheco/fheco.hpp"
#include <chrono>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
using namespace fheco;

// Slot count of the function being compiled, for kernels that rotate or loop
// over slots (rotations are cyclic over slots()).
static size_t g_slot_count = 0;
inline size_t slots()
{
  return g_slot_count;
}

#include "kernel.inc"

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    cerr << "usage: " << argv[0] << " <slot_count>\n";
    return 2;
  }
  const size_t slot_count = stoul(argv[1]);
  g_slot_count = slot_count;

  Compiler::enable_cse();
  Compiler::enable_order_operands();
  Compiler::enable_const_folding();

  // Shared CKKS parameters of the repository's CKKS benchmarks.
  CkksParams params{32768, {60, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 60}, 40};

  const auto start = chrono::high_resolution_clock::now();
  const auto &func = Compiler::create_ckks_func("fhe", slot_count, params);
  util::copyFile("fhe_io_example.txt", "fhe_io_example_adapted.txt");
  fhe();
  ofstream header("he/_gen_he_fhe.hpp");
  ofstream source("he/_gen_he_fhe.cpp");
  if (!header || !source)
    throw logic_error("failed to create he/_gen_he_fhe.*");
  Compiler::compile(func, Compiler::Ruleset::simplification_ruleset, trs::RewriteHeuristic::bottom_up);
  Compiler::gen_he_code(func, header, "_gen_he_fhe.hpp", source);
  const chrono::duration<double, milli> elapsed = chrono::high_resolution_clock::now() - start;
  cout << "Compile time : \n" << elapsed.count() << " ms\n";
  util::Quantifier quantifier{func};
  quantifier.run_all_analysis();
  quantifier.print_info(cout);
  return 0;
}
