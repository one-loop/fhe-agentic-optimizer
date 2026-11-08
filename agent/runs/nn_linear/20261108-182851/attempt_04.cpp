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
    size_t half = (in_features + 1) / 2;
    Ciphertext a = w[i][0] * x[0];
    for (size_t j = 1; j < half; ++j)
      a += (w[i][j] * x[j]);
    Ciphertext c = b[i];
    for (size_t j = half; j < in_features; ++j)
      c += (w[i][j] * x[j]);
    Ciphertext y = a + c;
    y.set_output("y_" + to_string(i));
  }
}
