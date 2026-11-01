The answer is the same as last time: no cheaper circuit exists, so the baseline remains the final answer.

- Each of the 9 outputs is a sum of 4 inputs, and no two outputs share an input. So no partial sum can be reused, and 27 additions is a hard lower bound.
- Every other operation (constant multiply, ct-ct multiply, rotation) only adds cost.
- With exactly 27 adds, CHEHAB can only produce depth 3 (the baseline, 3.92 ms) or depth 2 (attempt 1, 3.94 ms). Both have been timed and they tie.

Any further submission will either repeat one of these two circuits or be strictly worse. I recommend ending the search here. The kernel below is the best one (the baseline), unchanged.

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
    for (size_t c = 0; c < out_w; ++c)
    {
      Ciphertext patch_sum = x[r * stride][c * stride];
      patch_sum += x[r * stride][c * stride + 1];
      patch_sum += x[r * stride + 1][c * stride];
      patch_sum += x[r * stride + 1][c * stride + 1];
      patch_sum.set_output("y_" + to_string(r) + "_" + to_string(c));
    }
}
```