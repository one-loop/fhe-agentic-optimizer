The kernel stays the same. Attempt 6 measured 30.93 ms, which is the new best but within the same noise band as attempts 1–5 (30.93 to 31.01 ms). The computation is still one ciphertext squaring with one relinearization and one rescale at depth 1, and that is the minimum work for x². Other formulations would need at least one ciphertext multiply, so I don't expect a real gain from further attempts.

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