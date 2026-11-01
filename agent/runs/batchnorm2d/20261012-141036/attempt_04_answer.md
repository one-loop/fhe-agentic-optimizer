Attempt 3 gave 9.84 ms, the same as the baseline, with the same operation counts. The spread across attempts (9.79 to 9.87 ms) is measurement noise, not a kernel difference. I'm not proposing another variant because nothing would change the measured time.

The kernel does the minimum possible: one ciphertext-plaintext multiply and one plaintext add, with no rotations, ciphertext-ciphertext multiplies, or extra levels. I'm keeping the attempt 1 kernel as the final answer:

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