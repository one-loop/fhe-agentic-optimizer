I'm stopping here and resubmitting the attempt 1 kernel. Across six runs of the same single-multiply computation the median ranged from 63.8 to 64.7 ms, which is run-to-run noise. The kernel already performs the minimum work possible: one ciphertext squaring, one relinearize and one rescale, with no rotations or plaintext operations. Further attempts would only remeasure that same operation.

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