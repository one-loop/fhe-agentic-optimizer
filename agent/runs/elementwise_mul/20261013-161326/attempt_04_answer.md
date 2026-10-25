I'm stopping here and returning the best kernel found, which is the attempt 1 version. Four submissions have now produced the same circuit (one ciphertext-ciphertext multiply, one relinearization, one rescale, depth 1), and their timings (64.8 to 68.2 ms) vary only from run to run. A further attempt would only re-measure that noise.

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