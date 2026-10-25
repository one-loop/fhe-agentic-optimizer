The last two runs (19.90 and 20.29 ms) used code identical to the 18.14 ms run, so the variation is system noise. The graph is already minimal, with one ct-pt multiply and rescale plus one plaintext add, and I have no rewrite that removes an operation. For the final attempt I'm submitting the same baseline expression.

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