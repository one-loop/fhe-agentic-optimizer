I'm stopping here rather than spending more attempts. The baseline kernel is already the minimal evaluation of `y = x*A + B`: one ciphertext-plaintext multiply, one plaintext add, no rotations or ciphertext multiplications. Across three rewrites the measured time stayed between 17.74 and 19.02 ms, which is run-to-run noise, not a real difference. I don't see a legal reformulation that removes either operation, since plaintext-plaintext arithmetic and numeric constants are disallowed.

Best kernel (same as the baseline):

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