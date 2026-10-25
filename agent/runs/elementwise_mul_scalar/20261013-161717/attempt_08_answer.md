I'm keeping the kernel unchanged. It performs one ciphertext-plaintext multiply and one rescale, the minimum for `y = x0 * s`. The measured times (8.88–9.34 ms) are noise around the same operation, and I don't see a cheaper formulation in this DSL.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```