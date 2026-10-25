I'm not changing the kernel. It is one ciphertext-plaintext add, which is the minimum for `y = x0 + p`, and every run since the baseline has landed between 4.63 and 6.66 ms on identical work. The 4.70 ms result is within that noise band. The best measured time is attempt 5 at 4.63 ms, and the code below is that same kernel.

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