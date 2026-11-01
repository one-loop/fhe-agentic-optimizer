I'm keeping the same kernel. Attempts 1–3 all land between 30.95 and 31.01 ms, against a baseline of 31.41 ms, so the differences are run-to-run noise. The kernel is one ciphertext squaring with one relinearization and one rescale, which is the minimum for x². Any alternative would need at least one ciphertext multiply and would not beat this.

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