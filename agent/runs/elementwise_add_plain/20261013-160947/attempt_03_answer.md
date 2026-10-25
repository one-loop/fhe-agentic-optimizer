The kernel is already at its floor: one plaintext add with no multiplications, rotations, or level use. The timings (4.64 to 4.74 ms) are run-to-run noise on identical work, so no rewrite can meaningfully help. I'm resubmitting it unchanged as the best-known version.

```cpp
// Elementwise add plain: y = x0 + p. Already minimal (one add, no multiplications,
// no rotations, no level consumption); resubmitted unchanged.

void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```