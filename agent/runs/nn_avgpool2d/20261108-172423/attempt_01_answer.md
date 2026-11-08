The 9 output patches are disjoint, so no partial sums can be shared. Each output needs 3 additions, and 27 is the minimum count. The only change left is the shape of the sum. The baseline chains the adds (depth 3). A balanced tree `(a+b)+(c+d)` has depth 2 with the same add count, which gives a different circuit. I haven't run this, so I don't know whether it measures faster.

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
      Ciphertext top = x[r * stride][c * stride] + x[r * stride][c * stride + 1];
      Ciphertext bot = x[r * stride + 1][c * stride] + x[r * stride + 1][c * stride + 1];
      Ciphertext patch_sum = top + bot;
      patch_sum.set_output("y_" + to_string(r) + "_" + to_string(c));
    }
  }
}
```