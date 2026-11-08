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
    vector<Ciphertext> p;
    for (size_t j = 0; j < in_features; ++j)
      p.push_back(w[i][j] * x[j]);
    // first four products as a balanced pair, the rest chained, bias last
    Ciphertext s = (p[0] + p[1]) + (p[2] + p[3]);
    for (size_t j = 4; j < in_features; ++j)
      s = s + p[j];
    Ciphertext y = s + b[i];
    y.set_output("y_" + to_string(i));
  }
}
