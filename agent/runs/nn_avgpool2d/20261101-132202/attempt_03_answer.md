There is no different circuit left that could be cheaper. Every add-only circuit for this task has the same 27 additions, so the only property that can vary is the depth:

- **27 additions is the floor.** The nine output sums use disjoint sets of four inputs, so no partial sum can be shared. Each output needs exactly 3 additions.
- **Only two depths are possible.** A sum of four terms has depth 2 (balanced tree) or depth 3 (chain). Mixing the two shapes across outputs gives depth 3 with 27 adds, which is the same circuit as the baseline.
- **Both circuits have been timed.** The depth-3 chain is the baseline (3.92 ms). The depth-2 tree was attempt 1 (3.94 ms), which is the same within noise.
- **Every other operation adds cost.** Multiplying by constants, rotating, or packing values into slots can only make it slower. A rotation costs about 20 ms, against about 0.15 ms for an add.

The baseline is therefore optimal for this task, and further attempts cannot meet the "at least 3% faster" bar. Below is the depth-2 balanced tree again, as the structurally minimal kernel. It is the same circuit as attempt 1, so it will not be re-timed.

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
      Ciphertext top = x[r * stride][c * stride] + x[r * stride][c * stride + 1];
      Ciphertext bot = x[r * stride + 1][c * stride] + x[r * stride + 1][c * stride + 1];
      Ciphertext s = top + bot;
      s.set_output("y_" + to_string(r) + "_" + to_string(c));
    }
}
```