Ciphertext tree_sum(vector<Ciphertext> v)
{
  while (v.size() > 1)
  {
    vector<Ciphertext> nv;
    for (size_t k = 0; k + 1 < v.size(); k += 2)
      nv.push_back(v[k] + v[k + 1]);
    if (v.size() % 2 == 1)
      nv.push_back(v.back());
    v = nv;
  }
  return v[0];
}

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
    vector<Ciphertext> terms;
    for (size_t j = 0; j < in_features; ++j)
      terms.push_back(w[i][j] * x[j]);
    Ciphertext s = tree_sum(terms);
    Ciphertext y = s + b[i];
    y.set_output("y_" + to_string(i));
  }
}
