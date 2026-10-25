The kernel is already at its minimum: one plaintext multiply with rescale and one plaintext add. The run-to-run differences (18.1 to 18.5 ms) are noise, not real gains. Every change I considered either adds an operation or needs plaintext-plaintext arithmetic, which is not allowed. For example, folding B into the multiply as (x + B/A) * A would need a plaintext division, and it would also push the add to a higher level, where it costs more. So I'm submitting the same minimal form again.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```