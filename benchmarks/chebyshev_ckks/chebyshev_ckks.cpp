#include "chebyshev_coeffs.hpp"
#include "fheco/fheco.hpp"
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
using namespace fheco;

namespace
{
// Same threshold as the direct-SEAL reference: coefficients at or below it are
// fitting noise (even terms of an odd function) and get no term.
constexpr double coeff_epsilon = 1e-14;

const chehab::dl::ChebyshevCoeffSet &find_coeff_set(const string &activation, int degree)
{
  for (const auto &set : chehab::dl::chebyshev_coeff_sets())
  {
    if (activation == set.activation && degree == set.degree)
      return set;
  }
  throw invalid_argument("no committed coefficient set for " + activation + " degree " + to_string(degree));
}
} // namespace

// Chebyshev activation in the CHEHAB DSL, compiled for CKKS, with the same
// evaluation as the direct-SEAL reference (benchmarks/dl_operators_ckks):
//   z = alpha*x + beta, T1 = z, T2 = 2*z*z - 1, Tk = 2*z*T{k-1} - T{k-2},
//   P = sum_k c_k * T_k.
// alpha, beta, the coefficients c_k and the constant 1 are named plaintext
// inputs; 2*m is written m + m. The coefficient set only decides which terms
// exist. target selects the output: "z", "t2", "t3" (intermediate values,
// for validation) or "p" (the polynomial).
void fhe(const chehab::dl::ChebyshevCoeffSet &set, const string &target)
{
  const int degree = target == "z" ? 1 : target == "t2" ? 2 : target == "t3" ? 3 : set.degree;

  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;
  const double beta_value = -(set.range_max + set.range_min) / (set.range_max - set.range_min);
  if (abs(beta_value) > coeff_epsilon)
  {
    Plaintext beta("beta");
    z = z + beta;
  }
  if (target == "z")
  {
    z.set_output("y");
    return;
  }

  Plaintext one("one");
  vector<Ciphertext> t(degree + 1);
  t[1] = z;
  Ciphertext z_square = z * z;
  t[2] = z_square + z_square - one;
  for (int k = 3; k <= degree; ++k)
  {
    Ciphertext z_t = z * t[k - 1];
    t[k] = z_t + z_t - t[k - 2];
  }
  if (target == "t2" || target == "t3")
  {
    t[degree].set_output("y");
    return;
  }

  Ciphertext result;
  bool has_result = false;
  for (int k = 1; k <= degree; ++k)
  {
    if (abs(set.coeffs[k]) <= coeff_epsilon)
      continue;

    Plaintext c_k("c" + to_string(k));
    Ciphertext term = t[k] * c_k;
    result = has_result ? result + term : term;
    has_result = true;
  }
  if (!has_result)
    throw invalid_argument("polynomial has no non-negligible coefficient of degree >= 1");

  if (abs(set.coeffs[0]) > coeff_epsilon)
  {
    Plaintext c_0("c0");
    result = result + c_0;
  }
  result.set_output("y");
}

int main(int argc, char **argv)
{
  // ./chebyshev_ckks [target] [slot_count] [call_quantifier] [cse] [const_folding] [activation] [degree]
  string target = "p";
  if (argc > 1)
    target = argv[1];
  if (target != "z" && target != "t2" && target != "t3" && target != "p")
    throw invalid_argument("target must be z, t2, t3 or p");

  size_t slot_count = 9;
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

  // Any committed coefficient set (benchmarks/dl_operators_ckks/chebyshev_coeffs.hpp);
  // the default is the degree-5 Sigmoid of the direct-SEAL reference test.
  string activation = "sigmoid";
  if (argc > 6)
    activation = argv[6];

  int degree = 5;
  if (argc > 7)
    degree = stoi(argv[7]);

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

  const auto &set = find_coeff_set(activation, degree);

  // Same CKKS parameters as the direct-SEAL reference in
  // benchmarks/dl_operators_ckks: N = 32768, {60, 40 x 10, 60}, scale 2^40.
  CkksParams params{32768, {60, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 60}, 40};

  chrono::high_resolution_clock::time_point t;
  chrono::duration<double, milli> elapsed;
  string func_name = "fhe";
  t = chrono::high_resolution_clock::now();
  const auto &func = Compiler::create_ckks_func(func_name, slot_count, params);
  util::copyFile("fhe_io_example.txt", "fhe_io_example_adapted.txt");
  fhe(set, target);
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
