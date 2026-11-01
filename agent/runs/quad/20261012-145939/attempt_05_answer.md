The kernel stays the same. Attempt 4 measured 30.97 ms, against 30.95 ms for attempt 2 and 31.41 ms for the baseline. These differences are run-to-run noise, not a real change. The kernel is one ciphertext squaring with one relinearization and one rescale, and that is the minimum work for x². No rewrite can drop the multiply, and rotations or plaintext multiplies would only add cost, so I'm not spending more attempts on variants.

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