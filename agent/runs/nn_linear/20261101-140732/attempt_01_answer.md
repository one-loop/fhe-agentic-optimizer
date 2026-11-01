The 36 products cannot be reduced. A general matrix times a vector, with every entry encrypted, has bilinear rank exactly W·W, so 36 is the minimum for W = 6. Winograd's pairing trick does not help here: the weights are encrypted, so their pairwise products cannot be precomputed, and it would need 39 multiplications. Packing is also ruled out, because the function has a single slot, so rotations do nothing.

What can change is the shape of the addition tree. The baseline folds each row's sum into one serial chain, giving total depth 7. This kernel adds the products and the bias in a balanced pairwise tree, which should bring total depth down to about 4. The operation counts stay the same: 36 multiplications, 36 relinearizations and 36 additions. That gives CHEHAB's scheduler shorter dependency chains, and the circuit counts as distinct from the baseline. Any speedup is likely to be small, since the cost is dominated by the multiplications.

```cpp
Ciphertext tree_sum(vector<Ciphertext> v)
{
  while (v.size() > 1)
  {
    vector<Ciphertext> next;
    for (size_t k = 0; k + 1 < v.size(); k += 2)
      next.push_back(v[k] + v[k + 1]);
    if (v.size() % 2 == 1)
      next.push_back(v.back());
    v = next;
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
    terms.push_back(b[i]);
    Ciphertext y = tree_sum(terms);
    y.set_output("y_" + to_string(i));
  }
}
```