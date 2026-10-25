Attempt 4 measured 64.14 ms for the same `square(x)` kernel, so the variation across runs is about ±0.4 ms and no version beats the others. The computation is already at its floor: one ciphertext squaring with its relinearize and rescale, and no rotations or plaintext work. I'm resubmitting the kernel from attempt 1, which had the lowest measured time.

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