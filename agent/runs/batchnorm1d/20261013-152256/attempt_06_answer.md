The kernel is at its floor. Every attempt has run the same two operations, one ciphertext-plaintext multiply and one plaintext add, and the times (17.74 to 19.02 ms) are run-to-run noise. The only remaining lever would be removing an operation, and neither can go: A and B are per-slot vectors, so they can't become scalars, and the rules forbid plaintext-plaintext arithmetic and numeric constants. I'm keeping the baseline as the final answer and not spending the remaining attempts.

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