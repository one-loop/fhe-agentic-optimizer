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
    size_t head = in_features - 2;
    Ciphertext a = b[i];
    for (size_t j = 0; j < head; ++j)
      a += (w[i][j] * x[j]);
    Ciphertext c = w[i][head] * x[head];
    c += (w[i][head + 1] * x[head + 1]);
    Ciphertext y = a + c;
    y.set_output("y_" + to_string(i));
  }
}
