// Writes fhe_io_example.txt for the chebyshev_ckks benchmark (degree-5 Sigmoid
// on [-4, 4], the committed set in benchmarks/dl_operators_ckks).
//
//   ./generate_chebyshev_ckks <target> <points>
//
// target: z | t2 | t3 | p   (the value the compiled program outputs)
// points: points9  the nine points -4, -3, ..., 4 of the direct-SEAL reference
//         dense    256 seeded uniform points in [-4, 4]
//
// Inputs: ciphertext x; plaintexts alpha, beta, one, c0..c5 (each value
// repeated in every slot). The expected output is the same computation in
// plaintext double arithmetic, so comparing it with the decrypted output
// measures FHE numerical error only. Polynomial approximation error (the
// plaintext polynomial vs the true Sigmoid) is printed separately.
//
// Prints the slot count on the first line, then diagnostics.
#include "activations.hpp"
#include "chebyshev_coeffs.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
using namespace chehab::dl;

namespace
{
const ChebyshevCoeffSet &sigmoid_degree5()
{
  for (const auto &set : chebyshev_coeff_sets())
  {
    if (string(set.activation) == "sigmoid" && set.degree == 5)
      return set;
  }
  throw logic_error("missing sigmoid degree-5 coefficient set");
}

void write_values(FILE *f, const vector<double> &values)
{
  for (double v : values)
    fprintf(f, " %.17g", v);
  fprintf(f, "\n");
}
} // namespace

int main(int argc, char **argv)
{
  const string target = argc > 1 ? argv[1] : "p";
  const string points = argc > 2 ? argv[2] : "points9";
  if (target != "z" && target != "t2" && target != "t3" && target != "p")
  {
    cerr << "target must be z, t2, t3 or p\n";
    return 2;
  }

  const auto &set = sigmoid_degree5();
  const double lo = set.range_min, hi = set.range_max;

  vector<double> x;
  if (points == "points9")
  {
    for (int i = 0; i < 9; ++i)
      x.push_back(lo + (hi - lo) * i / 8.0);
  }
  else if (points == "dense")
  {
    mt19937 rng(5);
    uniform_real_distribution<double> dist(lo, hi);
    for (int i = 0; i < 256; ++i)
      x.push_back(dist(rng));
  }
  else
  {
    cerr << "points must be points9 or dense\n";
    return 2;
  }
  const size_t n = x.size();

  const double alpha = 2.0 / (hi - lo);
  const double beta = -(hi + lo) / (hi - lo);

  // Plaintext reference of each target, same recurrence as the encrypted
  // program; the polynomial includes every coefficient.
  vector<double> y(n), poly(n);
  for (size_t i = 0; i < n; ++i)
  {
    const double z = alpha * x[i] + beta;
    vector<double> t(set.degree + 1);
    t[0] = 1.0;
    t[1] = z;
    for (int k = 2; k <= set.degree; ++k)
      t[k] = 2.0 * z * t[k - 1] - t[k - 2];
    double p = 0;
    for (int k = 0; k <= set.degree; ++k)
      p += set.coeffs[k] * t[k];
    poly[i] = p;
    y[i] = target == "z" ? z : target == "t2" ? t[2] : target == "t3" ? t[3] : p;
  }

  double approx_error = 0;
  for (size_t i = 0; i < n; ++i)
    approx_error = max(approx_error, abs(poly[i] - activation_reference(set.activation, x[i])));

  FILE *f = fopen("fhe_io_example.txt", "w");
  if (!f)
    throw runtime_error("failed to create fhe_io_example.txt");
  const vector<pair<string, double>> scalars{
    {"alpha", alpha}, {"beta", beta}, {"one", 1.0},
    {"c0", set.coeffs[0]}, {"c1", set.coeffs[1]}, {"c2", set.coeffs[2]},
    {"c3", set.coeffs[3]}, {"c4", set.coeffs[4]}, {"c5", set.coeffs[5]}};
  fprintf(f, "%zu %zu 1\n", n, scalars.size() + 1);
  fprintf(f, "x 1 1");
  write_values(f, x);
  for (const auto &[label, value] : scalars)
  {
    fprintf(f, "%s 0 1", label.c_str());
    write_values(f, vector<double>(n, value));
  }
  fprintf(f, "y 1");
  write_values(f, y);
  fclose(f);

  cout << n << "\n";
  cout << "sigmoid degree " << set.degree << " on [" << lo << ", " << hi << "], target " << target << ", "
       << points << " (" << n << " points)\n";
  cout << "approximation error (plaintext polynomial vs sigmoid) on these points: " << approx_error << "\n";
  cout << "max_fit_error (fitting grid, chebyshev_coeffs.json): " << set.max_fit_error << "\n";
  return 0;
}
