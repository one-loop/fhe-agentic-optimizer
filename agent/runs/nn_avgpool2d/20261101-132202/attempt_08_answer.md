For the final attempt there is still no cheaper circuit, so I am submitting the baseline as the result.

- Each output sums 4 inputs that no other output uses, so each needs 3 additions and none can be shared. That makes 27 adds the minimum.
- With exactly 27 adds, the only circuits are depth 3 (the baseline, 3.92 ms) and depth 2 (attempt 1, 3.94 ms). They tie within noise.
- Any multiply or rotation would only add cost, and rotations are useless with 1 slot.

The baseline is optimal for this task, and it remains the best result.

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