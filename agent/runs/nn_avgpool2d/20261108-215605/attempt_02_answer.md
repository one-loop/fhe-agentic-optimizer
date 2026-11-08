No addition-only kernel can beat the baseline.

- **Why 27 additions is the floor:** the windows don't overlap, so each output is a sum of 4 inputs that no other output uses. Each output therefore needs exactly 3 additions, and nothing can be shared across outputs.
- **Why other operations don't help:** there are no multiplications to remove. Rotations (~20 ms each), plaintext operations (~1 ms or more) and packing would only add cost.
- **What attempt 1 showed:** the balanced tree changed the depth from 3 to 2 with the same 27 additions. It timed 8.80 ms against 7.27 ms, probably measurement noise, since both run the same 27 additions.

I'm returning the baseline unchanged. Its circuit is identical to the baseline's, so it cannot be judged a new best. Spending the remaining 6 attempts on more regroupings is unlikely to produce a confirmed 3% gain, so I recommend stopping here.

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