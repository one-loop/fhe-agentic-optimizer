I'm stopping the search here. The last three runs of the same `square(x)` kernel came in at 63.8–64.0 ms, and the baseline `x * x` measured 64.7 ms, so the spread is run-to-run noise rather than a real difference. The kernel already does the minimum work: one ciphertext squaring, one relinearize and one rescale, with no rotations or plaintext operations. Further attempts would only be remeasuring this same operation.

The best measured kernel is attempt 1, so this is the one I'm submitting:

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