The last run measured 4.64 ms against a 4.74 ms baseline. That gap is run-to-run noise, since the kernel is byte-identical. The graph has one ciphertext-plaintext add and nothing else, so there is no operation left to remove or move to a cheaper level. Any rewrite would add work, not reduce it. I'm resubmitting the same kernel.

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