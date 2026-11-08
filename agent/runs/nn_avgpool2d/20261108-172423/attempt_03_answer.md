I still have no circuit with fewer than 27 adds, because the patches are disjoint and nothing can be shared. Packing values would add constant multiplies and cost more than the adds it removes. This attempt only changes the pairing to `(a+c)+(b+d)`, which pairs vertical neighbours. It is the same depth and add count, so CHEHAB may treat it as a duplicate.

```cpp
void fhe()
{
  const size_t in_h = WIDTH, in_w = WIDTH;
  const size_t stride = 2;
  const size_t out_h = in_h / stride;
  const size_t out_w = in_w / stride;

  vector<vector<Ciphertext>> x(in_h, vector<Ciphertext>(in_w));
  for (size_t r = 0; r < in_h; ++r)
    for (size_t c = 0; c < in_w; ++c)
      x[r][c] = Ciphertext("x_" + to_string(r) + "_" + to_string(c));

  for (size_t r = 0; r < out_h; ++r)
  {
    for (size_t c = 0; c < out_w; ++c)
    {
      Ciphertext left = x[r * stride][c * stride] + x[r * stride + 1][c * stride];
      Ciphertext right = x[r * stride][c * stride + 1] + x[r * stride + 1][c * stride + 1];
      Ciphertext patch_sum = left + right;
      patch_sum.set_output("y_" + to_string(r) + "_" + to_string(c));
    }
  }
}
```