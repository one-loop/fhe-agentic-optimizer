I don't have a structurally different circuit that could be cheaper, and I won't invent a worse variant just to change the counts. Attempts 2 and 3 were resubmissions that couldn't win, and I should have said that more plainly.

The reason is a lower bound, not a lack of ideas:

- **Additions are at the minimum.** The nine outputs sum disjoint 4-element sets of inputs. Each needs at least 3 additions and none can share a partial sum, so 27 adds is the floor.
- **No other operation helps.** The only alternatives are constant multiplies (~1 ms each), ct-ct multiplies (~30 ms) or rotations (~20 ms, and useless with slots() = 1). Each of these only adds cost on top of the required adds.
- **Both possible circuits are measured.** With exactly 27 adds, the only reachable circuits are depth 3 (the baseline, 3.92 ms) and depth 2 (attempt 1, 3.94 ms). They tie within noise.

So the baseline is optimal for this task, and I recommend stopping here rather than spending the remaining 5 attempts. If the harness needs a kernel, here is the best one (the baseline):

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