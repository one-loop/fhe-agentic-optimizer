The kernel stays the same. Attempt 5 measured 30.96 ms, in line with attempts 1–4 (30.95 to 31.01 ms) and the baseline at 31.41 ms. The spread is noise. The computation is one ciphertext squaring, with one relinearization and one rescale at depth 1. A different formulation would still need a ciphertext multiply and could not drop below this, and rotations or plaintext multiplies would only add cost.

```cpp
// y = x^2: one ciphertext squaring (relinearize + rescale), depth 1.
// This is the minimum possible work for a quadratic activation.

void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```