I'm submitting the baseline as the final kernel. Seven attempts have compiled to the same circuit, and the timings (9.59 to 9.88 ms) show only drift.

The kernel does one ciphertext-plaintext multiply by `A` and one plaintext add of `B`. Both are required by `y = x * A + B`, and there are no rotations or ciphertext multiplies to remove.

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