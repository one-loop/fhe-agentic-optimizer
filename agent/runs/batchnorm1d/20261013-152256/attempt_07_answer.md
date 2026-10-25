I'm keeping the baseline as the final kernel. This attempt ran the same two operations again and measured 21.02 ms, so the spread across identical kernels (17.74 to 21.02 ms) is timing noise, not a property of the code. Nothing in the computation can be removed: A and B are per-slot vectors, plaintext-plaintext arithmetic and numeric constants are disallowed, and no rotations or ciphertext multiplications are needed.

```cpp
// y = x * A + B: one ciphertext-plaintext multiply and one plaintext add.
// This is the minimal evaluation; no rotations or ciphertext multiplications.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```