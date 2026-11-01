I'm stopping with the baseline. Five submissions have produced the identical circuit, and the timings (9.59 to 9.84 ms) show only measurement drift.

The kernel performs one ciphertext-plaintext multiply by `A` and one plaintext add of `B`. Both are required by the function, and it uses no rotations, ciphertext multiplies, or extra levels that could be removed.

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