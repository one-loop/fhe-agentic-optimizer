#include "fheco/fheco.hpp"

using namespace std;
using namespace fheco;
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "../global_variables.hpp"

/******************************************************************************
 * Orion-style nn.Conv2d operator in CHEHAB:
 * y(r, c) = b + \sum_{ki=0}^{2} \sum_{kj=0}^{2} k(ki, kj) * x(r+ki, c+kj)
 ******************************************************************************/

void fhe_vectorized(int width)
{
  Ciphertext img("img");
  Ciphertext b("b");
  Ciphertext result = b;
  for (int ki = 0; ki < 3; ++ki)
  {
    for (int kj = 0; kj < 3; ++kj)
    {
      Ciphertext k_val("k_" + to_string(ki) + "_" + to_string(kj));
      int shift = ki * width + kj;
      Ciphertext shifted = (shift == 0) ? img : (img >> shift);
      result += shifted * k_val;
    }
  }
  result.set_output("result");
}

void fhe(int width)
{
  size_t in_h = width;
  size_t in_w = width;
  size_t k_h = 3, k_w = 3;
  size_t out_h = in_h - k_h + 1;
  size_t out_w = in_w - k_w + 1;

  vector<vector<Ciphertext>> x(in_h, vector<Ciphertext>(in_w));
  for (size_t r = 0; r < in_h; ++r)
  {
    for (size_t c = 0; c < in_w; ++c)
    {
      x[r][c] = Ciphertext("x_" + to_string(r) + "_" + to_string(c));
    }
  }

  vector<vector<Ciphertext>> k(k_h, vector<Ciphertext>(k_w));
  for (size_t r = 0; r < k_h; ++r)
  {
    for (size_t c = 0; c < k_w; ++c)
    {
      k[r][c] = Ciphertext("k_" + to_string(r) + "_" + to_string(c));
    }
  }

  Ciphertext b("b");

  for (size_t r = 0; r < out_h; ++r)
  {
    for (size_t c = 0; c < out_w; ++c)
    {
      Ciphertext sum = b + (k[0][0] * x[r][c]);
      for (size_t ki = 0; ki < k_h; ++ki)
      {
        for (size_t kj = 0; kj < k_w; ++kj)
        {
          if (ki == 0 && kj == 0) continue;
          sum += (k[ki][kj] * x[r + ki][c + kj]);
        }
      }
      sum.set_output("y_" + to_string(r) + "_" + to_string(c));
    }
  }
}

void print_bool_arg(bool arg, const string &name, ostream &os)
{
  os << (arg ? name : "no_" + name);
}

int main(int argc, char **argv)
{
  bool vectorize_code = true;
  if (argc > 1)
    vectorize_code = stoi(argv[1]);

  int slot_count = 4;
  if (argc > 2)
    slot_count = stoi(argv[2]);

  int optimization_method = 0;  // 0 = egraph (default), 1 = RL
  if (argc > 3)
    optimization_method = stoi(argv[3]);

  int window = 0;
  if (argc > 4)
    window = stoi(argv[4]);

  bool call_quantifier = true;
  if (argc > 5)
    call_quantifier = stoi(argv[5]);

  bool cse = true;
  if (argc > 6)
    cse = stoi(argv[6]);

  bool const_folding = true;
  if (argc > 7)
    const_folding = stoi(argv[7]);

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

  chrono::high_resolution_clock::time_point t;
  chrono::duration<double, milli> elapsed;
  string func_name = "fhe";
  t = chrono::high_resolution_clock::now();

  if (vectorize_code)
  {
    const auto &func = Compiler::create_func(func_name, 1, 20, false, true);
    fhe(slot_count);
    string gen_name = "_gen_he_" + func_name;
    string gen_path = "he/" + gen_name;
    ofstream header_os(gen_path + ".hpp");
    if (!header_os)
      throw logic_error("failed to create header file");
    ofstream source_os(gen_path + ".cpp");
    if (!source_os)
      throw logic_error("failed to create source file");

    if (VECTORIZATION_ENABLED)
    {
      Compiler::gen_vectorized_code(func, window, optimization_method);
    }
    if (SIMPLIFICATION_ENABLED)
    {
      auto ruleset = Compiler::Ruleset::depth;
      auto rewrite_heuristic = trs::RewriteHeuristic::bottom_up;
      Compiler::compile(func, ruleset, rewrite_heuristic);
    }
    Compiler::gen_he_code(func, header_os, gen_name + ".hpp", source_os);

    elapsed = chrono::high_resolution_clock::now() - t;
    cout << elapsed.count() << " ms\n";
    if (call_quantifier)
    {
      util::Quantifier quantifier{func};
      quantifier.run_all_analysis();
      quantifier.print_info(cout);
    }
  }
  else
  {
    const auto &func = Compiler::create_func(func_name, slot_count, 20, false, true);
    std::string updated_inputs_file_name = "fhe_io_example_adapted.txt";
    std::string inputs_file_name = "fhe_io_example.txt";
    util::copyFile(inputs_file_name, updated_inputs_file_name);
    fhe(slot_count);
    string gen_name = "_gen_he_" + func_name;
    string gen_path = "he/" + gen_name;
    ofstream header_os(gen_path + ".hpp");
    if (!header_os)
      throw logic_error("failed to create header file");
    ofstream source_os(gen_path + ".cpp");
    if (!source_os)
      throw logic_error("failed to create source file");

    auto ruleset = Compiler::Ruleset::simplification_ruleset;
    auto rewrite_heuristic = trs::RewriteHeuristic::bottom_up;
    Compiler::compile(func, ruleset, rewrite_heuristic);
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
  }
  return 0;
}
