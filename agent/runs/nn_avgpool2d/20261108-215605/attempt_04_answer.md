There is still no cheaper circuit to propose, and the reasoning hasn't changed. The 9 outputs are sums over disjoint groups of 4 inputs. That forces 3 additions per output, 27 in total, with nothing shared and nothing to multiply. The only structural choices, depth 2 and depth 3, have both been tried. Any other circuit would add operations and run slower.

The baseline is optimal for this task. I'm returning it again as the final kernel; it will be reported as identical to the baseline. Further attempts won't change the result.

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