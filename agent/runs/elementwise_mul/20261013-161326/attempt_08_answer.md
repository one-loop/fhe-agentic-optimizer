I'm submitting the same kernel once more. Attempt 7 ran the identical circuit (one ciphertext-ciphertext multiply, one relinearization, one rescale, depth 1) and timed at 65.0 ms, within the same run-to-run spread as the other attempts. The product x0 * x1 requires exactly one ciphertext-ciphertext multiply, so no change to the circuit can lower the cost. Attempt 1 remains the best measured version.

```cpp
// Elementwise product: one ciphertext x ciphertext multiply (depth 1, relinearized, rescaled).
// This is the minimal circuit for x0 * x1 in CKKS.

void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```