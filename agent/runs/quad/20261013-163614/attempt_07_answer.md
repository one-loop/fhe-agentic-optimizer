Attempt 6 measured 63.71 ms, a new best, though the margin over the earlier runs is within noise. The kernel is unchanged: one ciphertext squaring with its relinearize and rescale, the minimum for x*x. I'm resubmitting it as the final answer.

```cpp
// y = x * x via the dedicated squaring operator: one ct-ct square, one relin + rescale.
// Minimal possible work for quadratic activation; no rotations or plaintext ops.

void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```