I haven't found a way to reduce the multiplication count, and the Winograd attempt confirmed that it only adds work. I'm falling back to one more add-structure variant. It keeps the bias chain from the baseline for `b_i` and the first four products. The last two products are summed separately and joined at the end, which gives total depth 6, a circuit not tried before. It still uses 36 multiplications and 36 adds.

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
```