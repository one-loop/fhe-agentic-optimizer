Attempt 1 changed only the addition tree, and the timing didn't move. That fits the cost model: each multiply-plus-relinearize is about 30 ms, while all 36 adds together come to roughly 11 ms. So only the number of multiplications and relinearizations really matters.

That number is hard to cut:

- **Tensor rank:** the product of an encrypted matrix and an encrypted vector has bilinear rank W², so no bilinear rewrite uses fewer than 36 products.
- **Winograd:** the commutative trick needs extra w·w products in every row, giving 39 or more.
- **Squares:** writing w·x as a combination of squares needs about 78 squares.
- **Packing:** with slots() = 1, rotations are the identity.

This attempt is therefore another structural variant, and I expect it to be no faster. I treat the bias as a seventh leaf in a balanced sum tree, which lowers the total depth to 4. Each row is built as one fused expression of products, in case CHEHAB's relinearization pass is lazier on that shape and can relinearize once per row instead of once per product. If neither change appears in the counts, the 36-multiply circuit is effectively optimal.

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
    terms.push_back(b[i]);
    for (size_t j = 0; j < in_features; ++j)
      terms.push_back(x[j] * w[i][j]);
    Ciphertext y = tree_sum(terms);
    y.set_output("y_" + to_string(i));
  }
}
```