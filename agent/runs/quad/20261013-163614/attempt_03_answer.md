The timing difference between runs is noise (63.8 to 64.0 ms for the same operation), so there is nothing left to optimize. The kernel performs one ciphertext squaring plus one relinearize and one rescale, which is the minimum for a quadratic activation. I'm submitting the same kernel as the best attempt.

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