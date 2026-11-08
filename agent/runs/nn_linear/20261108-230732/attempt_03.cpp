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
    // pairwise sums of products, then the pair sums, bias joins the last level
    vector<Ciphertext> pairs;
    size_t j = 0;
    for (; j + 1 < in_features; j += 2)
      pairs.push_back(w[i][j] * x[j] + w[i][j + 1] * x[j + 1]);
    if (j < in_features)
      pairs.push_back(w[i][j] * x[j]);
    pairs.push_back(b[i]);
    while (pairs.size() > 1)
    {
      vector<Ciphertext> nv;
      for (size_t k = 0; k + 1 < pairs.size(); k += 2)
        nv.push_back(pairs[k] + pairs[k + 1]);
      if (pairs.size() % 2 == 1)
        nv.push_back(pairs.back());
      pairs = nv;
    }
    pairs[0].set_output("y_" + to_string(i));
  }
}
