// Compiles one candidate CHEHAB DSL kernel, exactly like the repository
// benchmarks: Compiler::compile (simplification ruleset, bottom-up) then
// Compiler::gen_he_code, then the Quantifier report.
//
//   ./agent_kernel <slot_count> [ckks|bfv]   (run inside a directory with fhe_io_example.txt and he/)
//
// ckks (default): the shared CKKS parameters of benchmarks/*_ckks.
// bfv: the scalar BFV path of benchmarks/nn_* (create_func with a 20-bit
//      plaintext modulus, unsigned, cyclic rotations), as run_benchmarks.py
//      records it with --vectorize 0.
// The kernel (kernel.inc, written by agent/loop.py) defines `void fhe()`.
#include "fheco/fheco.hpp"
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdlib>
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

// Width of a BFV operator task (nn_* kernels: in/out features or image width),
// from AGENT_WIDTH in the environment, set by agent/evaluate.py per case.
static size_t g_width = 0;
#define WIDTH (g_width)

#include "kernel.inc"

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    cerr << "usage: " << argv[0] << " <slot_count> [ckks|bfv]\n";
    return 2;
  }
  const size_t slot_count = stoul(argv[1]);
  g_slot_count = slot_count;
  if (const char *w = getenv("AGENT_WIDTH"))
    g_width = stoul(w);
  const string scheme = argc > 2 ? argv[2] : "ckks";
  if (scheme != "ckks" && scheme != "bfv")
  {
    cerr << "scheme must be ckks or bfv\n";
    return 2;
  }

  Compiler::enable_cse();
  Compiler::enable_order_operands();
  Compiler::enable_const_folding();

  const auto start = chrono::high_resolution_clock::now();
  if (scheme == "ckks")
  {
    // Shared CKKS parameters of the repository's CKKS benchmarks.
    CkksParams params{32768, {60, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 60}, 40};
    // Size the modulus chain to the kernel, so that unused levels neither slow a
    // kernel down nor can be "burned" for speed (see agent/README.md).
    params.trim_to_levels_used = true;
    Compiler::create_ckks_func("fhe", slot_count, params);
  }
  else
  {
    // Same call as benchmarks/nn_*/nn_*.cpp on the scalar path.
    Compiler::create_func("fhe", slot_count, 20, false, true);
  }
  const auto &func = Compiler::active_func();
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
