I'm keeping the baseline as the final kernel. Across six attempts the circuit has been identical, and the timings (9.59 to 9.87 ms) are drift around the same two operations.

The kernel does one ciphertext-plaintext multiply by `A` and one plaintext add of `B`. Both are required by `y = x * A + B`. The only algebraic alternative, `(x + B/A) * A`, needs division, which CKKS does not support.

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