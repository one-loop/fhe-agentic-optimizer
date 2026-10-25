I'm finishing with the attempt 6 kernel, which had the best measured median at 63.71 ms. The timings for this same computation span about 63.7 to 64.7 ms across runs, so the differences are noise. The kernel already does the minimum: one ciphertext squaring, one relinearize and one rescale, with no rotations or plaintext operations. I'm not spending the last attempt on a variant that cannot do less work.

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