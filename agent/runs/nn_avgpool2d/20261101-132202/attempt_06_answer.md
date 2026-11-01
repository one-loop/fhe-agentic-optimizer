There is no change left to make: the baseline is optimal for this task, and the remaining 3 attempts cannot produce a faster circuit.

- **27 additions is the minimum.** Each output sums 4 inputs that belong to no other output, so it needs 3 additions and none can be shared.
- **Every other operation adds cost.** Constant multiplies, ct-ct multiplies and rotations only make the circuit slower, and rotations are useless with 1 slot.
- **Both possible circuits have been timed.** With 27 adds the depth can only be 3 (the baseline, 3.92 ms) or 2 (attempt 1, 3.94 ms), and the two tie.

I suggest ending the search. The kernel below is the best one (the baseline), unchanged.

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