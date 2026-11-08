I can't offer a cheaper circuit, because none exists for this task.

- **Additions:** the 2×2 windows with stride 2 don't overlap. Each of the 9 outputs is a sum of 4 inputs that no other output uses, so it needs exactly 3 additions. That makes 27 the floor.
- **Other operations:** there are no multiplications to remove. Constant multiplies (~1 ms), rotations (~20 ms) and plaintext operations would only add cost.
- **Depth:** a 4-term sum has depth 2 or 3. Both versions have now been run: attempt 1 (depth 2, 8.80 ms) and the baseline (depth 3, 7.27 ms).

Every other kernel either duplicates one of these circuits, which can't be timed as new, or adds operations, which would be slower. I recommend stopping here and keeping the baseline as the final answer. It is resubmitted below unchanged as the best kernel. It is not a new candidate and will again be reported as identical to the baseline.

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