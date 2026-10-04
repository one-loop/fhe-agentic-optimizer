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

const vector<string> ops{
  "extract", "extract_linear", "extract_sparse", "extract_sparse_boundary", "table_mult", "embedding"};

void fhe(const string &op, size_t slot_count)
{
  if (op == "extract" || op == "extract_linear")
  {
    Ciphertext input("input");
    Plaintext mask("mask");
    const size_t offset = slot_count / 4;
    const size_t extracted_size = slot_count / 2;
    auto output = op == "extract"
                    ? operators::extract(input, mask, slot_count, extracted_size, offset)
                    : operators::extract_linear(input, mask, slot_count, extracted_size, offset);
    output.set_output("output");
  }
  else if (op == "extract_sparse" || op == "extract_sparse_boundary")
  {
    Ciphertext first("first");
    Ciphertext second("second");
    Plaintext keep_front("keep_front");
    Plaintext drop_front("drop_front");
    const size_t num_dense = slot_count / 4;
    const size_t logical_size = op == "extract_sparse_boundary"
                                  ? slot_count + num_dense
                                  : slot_count + slot_count / 2;
    auto output = operators::extract_sparse(
      {{first, second}, logical_size}, keep_front, drop_front, num_dense, logical_size - num_dense);
    for (size_t i = 0; i < output.chunks.size(); ++i)
      output.chunks[i].set_output("output_" + to_string(i));
  }
  else
  {
    Ciphertext indicator("indicator");
    vector<Plaintext> table_columns;
    for (int i = 0; i < 3; ++i)
      table_columns.emplace_back("table_" + to_string(i));

    if (op == "table_mult")
    {
      auto output = operators::table_mult(indicator, table_columns);
      for (size_t i = 0; i < output.size(); ++i)
        output[i].set_output("output_" + to_string(i));
    }
    else
    {
      Plaintext first_slot("first_slot");
      operators::embedding(indicator, table_columns, first_slot).set_output("output");
    }
  }
}

int main(int argc, char **argv)
{
  string op = "extract";
  if (argc > 1)
    op = argv[1];
  if (find(ops.begin(), ops.end(), op) == ops.end())
    throw invalid_argument("unknown extract/embedding operation");

  size_t slot_count = 8;
  if (argc > 2)
    slot_count = stoul(argv[2]);
  const bool call_quantifier = argc <= 3 || stoi(argv[3]);
  const bool cse = argc <= 4 || stoi(argv[4]);
  const bool const_folding = argc <= 5 || stoi(argv[5]);

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

  CkksParams params{32768, {60, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 60}, 40};
  const auto start = chrono::high_resolution_clock::now();
  const auto &func = Compiler::create_ckks_func("fhe", slot_count, params);
  util::copyFile("fhe_io_example.txt", "fhe_io_example_adapted.txt");
  fhe(op, slot_count);

  ofstream header("he/_gen_he_fhe.hpp");
  ofstream source("he/_gen_he_fhe.cpp");
  if (!header || !source)
    throw logic_error("failed to create generated files");
  Compiler::compile(func, Compiler::Ruleset::simplification_ruleset, trs::RewriteHeuristic::bottom_up);
  Compiler::gen_he_code(func, header, "_gen_he_fhe.hpp", source);
  const chrono::duration<double, milli> elapsed = chrono::high_resolution_clock::now() - start;
  cout << "Compile time :\n" << elapsed.count() << " ms\n";
  if (call_quantifier)
  {
    util::Quantifier quantifier{func};
    quantifier.run_all_analysis();
    quantifier.print_info(cout);
  }
}
