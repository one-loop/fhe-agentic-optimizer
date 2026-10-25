The baseline already uses the minimum possible work: one ciphertext-plaintext addition, with no multiplications, rotations, or level consumption. Any alternative would add operations, so I'm resubmitting it unchanged. The remaining cost is the plaintext encoding and the single add, which the DSL doesn't let me avoid.

```cpp
// Elementwise add plain: y = x0 + p. Already minimal (one add, no multiplications,
// no rotations, no level consumption), so the kernel is unchanged from the baseline.

void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```