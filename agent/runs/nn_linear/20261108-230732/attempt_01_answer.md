The output is y_i = b_i + Σ_j w_ij·x_j, where both w and x are encrypted. Two obvious ways to cut multiplications don't work here:

- **Fewer products:** the encrypted matrix–vector product has bilinear rank W², so it needs W² products. Winograd's commutative trick would need extra w·w products in every row, which is worse.
- **Packing:** every value sits in its own 1-slot ciphertext, so rotations do nothing and the values can't be packed.

What can still change is the addition structure. The baseline builds each row as a 7-deep chain of adds. This kernel sums each row's products with a balanced tree and adds the bias last. That lowers the total depth from 7 to about 4 for W=6. It also gives CHEHAB's rewriter, including any lazy-relinearization it may apply, a cleaner sum-of-products to work on.

The gain may be small and might not clear the 3% bar.

```cpp
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
```