The result is 30.95 ms, down from 31.41 ms in the baseline, but the gap is noise. The kernel is the same single squaring with one relinearization and one rescale, so there is nothing left to remove. A different formulation would still need a ciphertext multiply, and rotations or plaintext multiplies would only add cost. I'm stopping here and keeping this kernel as the final answer.

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