I'm keeping the same kernel. Attempt 5 produced the identical circuit (one ciphertext-ciphertext multiply, one relinearization, one rescale, depth 1) and timed at 68.1 ms, which is the same run-to-run variation seen in attempts 1 to 4 (64.8 to 68.2 ms). No change to the circuit can reduce the cost, because a single ciphertext multiply is the minimum for x0 * x1. Attempt 1 is the best measured version, so it is returned again.

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