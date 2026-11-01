I'm making no further changes. The circuit is the same in every attempt, and the times (9.59 to 9.81 ms) are noise around the same two operations.

The kernel does one ciphertext-plaintext multiply by `A` and one plaintext add of `B`. Both are required by the function, and there are no rotations, ciphertext multiplies, or extra levels to remove. The only alternative, `(x + B/A) * A`, needs division, which CKKS does not support.

The best kernel is the baseline, which I'm resubmitting:

```cpp
// y = x * A + B: one plaintext multiply (depth 1) and one plaintext add.
// Already minimal for this function; no rotations or ciphertext multiplies.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```