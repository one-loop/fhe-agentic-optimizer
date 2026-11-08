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
    terms.push_back(b[i]);
    while (terms.size() > 1)
    {
      vector<Ciphertext> next;
      for (size_t k = 0; k + 1 < terms.size(); k += 2)
        next.push_back(terms[k] + terms[k + 1]);
      if (terms.size() % 2 == 1)
        next.push_back(terms.back());
      terms = next;
    }
    Ciphertext y = terms[0];
    y.set_output("y_" + to_string(i));
  }
}
