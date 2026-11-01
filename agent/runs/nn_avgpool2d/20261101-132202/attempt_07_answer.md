The baseline is still the answer, and nothing in the last two attempts can beat it.

- **27 additions is a hard lower bound.** Each of the 9 outputs sums 4 inputs that no other output uses, so each needs 3 additions and none can be shared.
- **Only two circuits are possible, and both are measured.** With 27 adds the depth can only be 3 (the baseline, 3.92 ms) or 2 (attempt 1, 3.94 ms), and they tie within noise.
- **Every other change is slower.** Adding constant multiplies, ct-ct multiplies or rotations only increases the cost.

I recommend ending the search here. This is the best kernel (the baseline):

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