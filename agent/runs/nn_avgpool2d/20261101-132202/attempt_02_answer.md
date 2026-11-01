The baseline is already optimal here, and no remaining attempt can beat it under the judging rules:

- **The add count cannot drop.** Each output sums 4 distinct inputs, and the 2x2 windows do not overlap, so no partial sum can be shared. That makes 3 additions per output, 27 in total, the minimum.
- **No other operation helps.** Multiplications and plaintext operations do nothing useful for a pure sum. Rotations cost about 20 ms each against about 0.15 ms per add, so packing values into slots would be far slower.
- **Every possible circuit is used up.** With 27 adds and nothing else, the only circuits are depth 3 (the baseline's chain) and depth 2 (the balanced tree from attempt 1). The balanced tree measured 3.94 ms against 3.92 ms, which is within noise. Any circuit with more adds would only be slower.

I am returning the best kernel (the baseline) unchanged.

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
      Ciphertext patch_sum = x[r * stride][c * stride];
      patch_sum += x[r * stride][c * stride + 1];
      patch_sum += x[r * stride + 1][c * stride];
      patch_sum += x[r * stride + 1][c * stride + 1];
      patch_sum.set_output("y_" + to_string(r) + "_" + to_string(c));
    }
  }
}
```