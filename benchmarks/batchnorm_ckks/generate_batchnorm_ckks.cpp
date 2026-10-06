// Writes fhe_io_example.txt for the batchnorm_ckks benchmark.
//
//   ./generate_batchnorm_ckks <case>
//
// cases:
//   bn1d       BatchNorm1d test data of the direct-SEAL reference (N=1, C=2, L=4)
//   bn2d       BatchNorm2d test data of the direct-SEAL reference (N=1, C=2, H=W=2)
//   bn1d_rand  seeded random BatchNorm1d, N=2, C=3, L=8
//   bn2d_rand  seeded random BatchNorm2d, N=2, C=3, H=W=4
//
// Inputs: ciphertext x, plaintexts A and B. A and B are produced by the
// reference module's folding and packing helpers (precompute_batch_norm,
// expand_batchnorm{1d,2d}_channels). The expected output y is computed
// independently from the original eval-mode formula
// gamma * (x - running_mean) / sqrt(running_var + eps) + beta, with each slot's
// channel derived from its index. Prints the slot count on the first line.
#include "fhe_operators.hpp"
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
struct BatchNormCase
{
  bool is_2d;
  size_t batch, channels, length, height, width;
  vector<double> x, gamma, beta, running_mean, running_var;
  double eps = 1e-5;

  size_t spatial_size() const { return is_2d ? height * width : length; }
  size_t size() const { return batch * channels * spatial_size(); }
};

BatchNormCase reference_case(bool is_2d)
{
  // Same tensors as benchmarks/dl_operators_ckks/main.cpp.
  BatchNormCase c;
  c.is_2d = is_2d;
  c.batch = 1;
  c.channels = 2;
  c.length = 4;
  c.height = 2;
  c.width = 2;
  c.x = {-1.0, -0.5, 0.0, 0.5, 1.0, 1.5, 2.0, 2.5};
  if (is_2d)
  {
    c.gamma = {1.1, 0.7};
    c.beta = {0.0, 0.2};
    c.running_mean = {0.1, 1.0};
    c.running_var = {0.9, 0.6};
  }
  else
  {
    c.gamma = {1.2, 0.8};
    c.beta = {0.1, -0.2};
    c.running_mean = {0.25, 1.25};
    c.running_var = {0.5, 0.75};
  }
  return c;
}

BatchNormCase random_case(bool is_2d)
{
  BatchNormCase c;
  c.is_2d = is_2d;
  c.batch = 2;
  c.channels = 3;
  c.length = 8;
  c.height = 4;
  c.width = 4;
  mt19937 rng(is_2d ? 2 : 1);
  auto uniform = [&rng](double lo, double hi, size_t n) {
    uniform_real_distribution<double> dist(lo, hi);
    vector<double> v(n);
    for (auto &e : v)
      e = dist(rng);
    return v;
  };
  c.gamma = uniform(0.5, 1.5, c.channels);
  c.beta = uniform(-0.5, 0.5, c.channels);
  c.running_mean = uniform(-1.0, 1.0, c.channels);
  c.running_var = uniform(0.1, 2.0, c.channels);
  c.x = uniform(-3.0, 3.0, c.size());
  return c;
}

vector<double> original_formula(const BatchNormCase &c)
{
  vector<double> y(c.x.size());
  for (size_t i = 0; i < c.x.size(); ++i)
  {
    const size_t ch = (i / c.spatial_size()) % c.channels;
    y[i] = c.gamma[ch] * (c.x[i] - c.running_mean[ch]) / sqrt(c.running_var[ch] + c.eps) + c.beta[ch];
  }
  return y;
}

vector<double> expand(const BatchNormCase &c, const vector<double> &per_channel)
{
  return c.is_2d ? expand_batchnorm2d_channels(per_channel, c.batch, c.channels, c.height, c.width)
                 : expand_batchnorm1d_channels(per_channel, c.batch, c.channels, c.length);
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
  const string name = argc > 1 ? argv[1] : "bn1d";
  BatchNormCase c;
  if (name == "bn1d")
    c = reference_case(false);
  else if (name == "bn2d")
    c = reference_case(true);
  else if (name == "bn1d_rand")
    c = random_case(false);
  else if (name == "bn2d_rand")
    c = random_case(true);
  else
  {
    cerr << "unknown case " << name << " (bn1d, bn2d, bn1d_rand, bn2d_rand)\n";
    return 2;
  }
  if (c.x.size() != c.size())
    throw logic_error("x size does not match the tensor shape");

  BatchNormParams folded = precompute_batch_norm(c.gamma, c.beta, c.running_mean, c.running_var, c.eps);
  const vector<double> a = expand(c, folded.scale);
  const vector<double> b = expand(c, folded.shift);
  const vector<double> y = original_formula(c);

  double fold_diff = 0;
  for (size_t i = 0; i < y.size(); ++i)
    fold_diff = max(fold_diff, abs(a[i] * c.x[i] + b[i] - y[i]));

  FILE *f = fopen("fhe_io_example.txt", "w");
  if (!f)
    throw runtime_error("failed to create fhe_io_example.txt");
  fprintf(f, "%zu 3 1\n", c.size());
  fprintf(f, "x 1 1");
  write_values(f, c.x);
  fprintf(f, "A 0 1");
  write_values(f, a);
  fprintf(f, "B 0 1");
  write_values(f, b);
  fprintf(f, "y 1");
  write_values(f, y);
  fclose(f);

  cout << c.size() << "\n";
  cout << name << ": " << (c.is_2d ? "BatchNorm2d NCHW " : "BatchNorm1d NCL ") << c.batch << "x" << c.channels << "x"
       << (c.is_2d ? to_string(c.height) + "x" + to_string(c.width) : to_string(c.length))
       << ", plaintext max |A*x+B - original formula| = " << fold_diff << "\n";
  return 0;
}
