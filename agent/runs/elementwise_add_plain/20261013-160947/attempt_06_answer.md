This run measured 4.63 ms, the new best, but the kernel is unchanged and the difference from the 4.64 ms run is noise. The graph is one ciphertext-plaintext add, which is the minimum for `y = x0 + p`. No rewrite can drop the add or the plaintext encoding, so I'm resubmitting the same kernel.

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