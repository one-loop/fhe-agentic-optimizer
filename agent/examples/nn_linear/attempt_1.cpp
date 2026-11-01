// Baseline: nn.Linear on the scalar BFV path, as benchmarks/nn_linear/nn_linear.cpp (fhe()).
// WIDTH is the case's in/out feature count (4 or 6), set by the harness.
void fhe()
{
  const size_t in_features = WIDTH;
  const size_t out_features = WIDTH;

  vector<Ciphertext> x(in_features);
  for (size_t j = 0; j < in_features; ++j)
    x[j] = Ciphertext("x_" + to_string(j));

  vector<vector<Ciphertext>> w(out_features, vector<Ciphertext>(in_features));
  vector<Ciphertext> b(out_features);
  for (size_t i = 0; i < out_features; ++i)
  {
    b[i] = Ciphertext("b_" + to_string(i));
    for (size_t j = 0; j < in_features; ++j)
      w[i][j] = Ciphertext("w_" + to_string(i) + "_" + to_string(j));
  }

  for (size_t i = 0; i < out_features; ++i)
  {
    Ciphertext y = b[i] + (w[i][0] * x[0]);
    for (size_t j = 1; j + 1 < in_features; ++j)
      y += (w[i][j] * x[j]);
    y.set_output("y_" + to_string(i));
  }
}
