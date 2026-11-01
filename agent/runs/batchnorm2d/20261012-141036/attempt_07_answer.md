Attempt 6 measured 9.69 ms, the new best, with the same operation counts. The kernel is unchanged, so the gain is again measurement variance, not a real improvement. I'm not changing the code. This is the final kernel:

```cpp
// y = x * A + B: one ciphertext-plaintext multiply (depth 1) plus one plaintext add.
// Minimal evaluation for this function; no further reduction in operations is possible.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```