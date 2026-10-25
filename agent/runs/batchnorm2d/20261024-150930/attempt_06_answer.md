I don't have a structurally cheaper circuit. The multiply by A (one rescale) and the add of B are both required, and every identity I tried needs plaintext-plaintext arithmetic or a constant, which CKKS here forbids. I'm resubmitting the minimal form, which will most likely be reported as identical to the baseline.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = a * x + b;
  y.set_output("y");
}
```