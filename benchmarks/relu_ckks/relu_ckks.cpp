#include "fheco/fheco.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
using namespace fheco;

namespace
{
// Odd polynomial sum_k c_{offset+k} t^k, k = 1, 3, ..., degree, with plaintext
// coefficients s<stage>c<index>, split baby-step/giant-step:
//   degree 1: c1 t        degree 3: c1 t + (c3 t) t^2
//   degree d >= 5: low(t) + t^m high(t), m the largest power of two below d,
//   low of degree m - 1 and high of degree d - m.
// A degree-d polynomial consumes ceil(log2(d + 1)) levels, the coefficient
// multiplications included, and every addition joins two operands at
// different levels, which align_ckks_operands lines up without an extra level
// (equal-level operands with different scales would need one).
class OddPolynomial
{
public:
  OddPolynomial(const Ciphertext &t, int degree, int stage) : stage_{stage}, pow2_{t}
  {
    // pow2_[j] = t^(2^j)
    while ((1 << pow2_.size()) < degree)
      pow2_.push_back(pow2_.back() * pow2_.back());
  }

  Ciphertext eval(int offset, int degree) const
  {
    const Ciphertext &t = pow2_[0];
    if (degree == 1)
      return t * coeff(offset + 1);
    if (degree == 3)
      return t * coeff(offset + 1) + (t * coeff(offset + 3)) * pow2_[1];

    size_t j = 0;
    while ((2 << j) < degree)
      ++j;
    const int m = 1 << j;
    return eval(offset, m - 1) + eval(offset + m, degree - m) * pow2_[j];
  }

private:
  Plaintext coeff(int k) const { return Plaintext("s" + to_string(stage_) + "c" + to_string(k)); }

  int stage_;
  vector<Ciphertext> pow2_;
};
} // namespace

// ReLU in the CHEHAB DSL, compiled for CKKS, through a composite minimax
// approximation of sign: ReLU(x) = x * (1 + sign(x / B)) / 2, with
// sign ~= p_n o ... o p_1 (odd minimax stages; fit_composite_sign.py). The
// input scaling 1/B is folded into p_1's coefficients and the 1/2 into p_n's,
// so the program computes h = p_n'(... p_1'(x)) + half and y = x * h.
// Coefficients and half are plaintext scalars of the io file.
void fhe(const vector<int> &degrees)
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (size_t i = 0; i < degrees.size(); ++i)
    t = OddPolynomial(t, degrees[i], static_cast<int>(i) + 1).eval(0, degrees[i]);
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}

int main(int argc, char **argv)
{
  // ./relu_ckks [slot_count] [call_quantifier] [cse] [const_folding] [stage degrees...]
  size_t slot_count = 9;
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

  // Odd degree of each composite sign stage; must match the io file's
  // coefficients (relu_sign_coeffs.json). Default: three degree-7 stages.
  vector<int> degrees;
  for (int i = 5; i < argc; ++i)
    degrees.push_back(stoi(argv[i]));
  if (degrees.empty())
    degrees = {7, 7, 7};
  for (int degree : degrees)
  {
    if (degree < 1 || degree % 2 == 0)
      throw invalid_argument("stage degrees must be odd and >= 1");
  }

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
  fhe(degrees);
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
