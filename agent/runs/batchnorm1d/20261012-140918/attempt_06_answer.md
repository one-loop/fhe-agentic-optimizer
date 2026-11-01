I'm keeping the baseline as the final kernel. Every attempt has produced the same circuit, and the timings (9.59 to 9.86 ms) differ only by noise.

The kernel does one ciphertext-plaintext multiply by `A` and one plaintext add of `B`. Both are required by `y = x * A + B`. Removing either would change the function, and the only algebraic alternative, `(x + B/A) * A`, needs division, which CKKS does not support.

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