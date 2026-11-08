The 2×2 windows with stride 2 don't overlap, so no partial sums can be shared between outputs. Each output needs exactly 3 ciphertext additions, so 27 additions is already the minimum. The only structural change left is a balanced tree, `(a + b) + (c + d)`, instead of a left-leaning chain. This lowers the addition depth from 3 to 2 with the same operation count, which gives a different circuit. Any speedup will be small at best, because the time is dominated by 27 additions of about 0.3 ms each.

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