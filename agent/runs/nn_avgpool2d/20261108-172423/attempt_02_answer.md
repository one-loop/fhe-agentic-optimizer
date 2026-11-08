I don't see a cheaper circuit. The 9 patches share no inputs, so no partial sum can be reused. Each output must combine 4 distinct ciphertexts, which needs at least 3 additions, and 27 is therefore the floor. Packing values with rotations or masks would cost ct-pt multiplies (about 1 ms each or more) and rotations, which are far more expensive than the 0.3 ms adds they would save. I'm resubmitting the balanced-tree kernel (depth 2, 27 adds). It may not count as a new circuit, so it probably can't be confirmed as an improvement.

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