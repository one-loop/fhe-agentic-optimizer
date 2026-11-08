Attempt 7 was another duplicate. I sent attempt 2's kernel back "for reference", and it was evaluated as a submission. That is the fifth wasted attempt, and the fault is mine.

There is no cheaper circuit for this task:
- **Multiplications:** each output y_i needs the 6 products w_ij·x_j of two independently encrypted values, so all 36 ciphertext-ciphertext multiplications are required.
- **Tricks that reorganize the products all cost more:**
  - Winograd pairing needs 39 or more.
  - Squaring identities need 72 or more.
  - Constant-K packing needs 11 products per row.
- **Additions:** 36 is already the minimum.
- **Rotations:** no help with one value per ciphertext.
- **Relinearizations:** CHEHAB inserts these itself.
- **Depth:** lowering it from 7 to 4 changed nothing measurable.

With the last attempt, this kernel at least has a shape not tried before. Products are summed pairwise, then chained, with the bias added last, giving a total depth of (6, 1), which differs from 7, 5 and 4. It has the same 36 multiplications, 36 relinearizations and 36 adds, so I don't expect it to beat the baseline. The baseline should stand as the result.

```cpp
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
```