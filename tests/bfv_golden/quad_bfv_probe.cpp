// BFV regression probe: the Quad program (y = x * x) compiled through the
// existing BFV path. Its generated code is part of the BFV golden snapshot.
#include "fheco/fheco.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace fheco;

int main()
{
  Compiler::enable_cse();
  Compiler::enable_order_operands();
  Compiler::enable_const_folding();

  const auto &func = Compiler::create_func("fhe", 8, 20, false, true);
  Ciphertext x("x");
  auto y = x * x;
  y.set_output("y");

  Compiler::compile(func, Compiler::Ruleset::simplification_ruleset, trs::RewriteHeuristic::bottom_up);

  std::ofstream header_os("he/_gen_he_fhe.hpp");
  std::ofstream source_os("he/_gen_he_fhe.cpp");
  if (!header_os || !source_os)
    throw std::logic_error("failed to create generated files");
  Compiler::gen_he_code(func, header_os, "_gen_he_fhe.hpp", source_os);

  util::Quantifier quantifier{func};
  quantifier.run_all_analysis();
  quantifier.print_info(std::cout);
  return 0;
}
