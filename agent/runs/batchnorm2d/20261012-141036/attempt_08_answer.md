Attempt 7 measured 9.83 ms with identical operation counts, so the kernel is unchanged and the spread is noise. The best measured time is attempt 6 (9.69 ms), and the code is the same, so I'm submitting it as the final answer:

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