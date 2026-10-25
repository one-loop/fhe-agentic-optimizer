I'm resubmitting the same kernel. It already does the minimum: a single ciphertext-plaintext multiply with one rescale and no rotations, relinearization, or alignment. The five measured times (8.92–9.34 ms) fall within run-to-run noise, and I don't see a cheaper formulation of `x0 * s` in this DSL.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```