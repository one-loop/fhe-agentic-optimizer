I have no further correct restructuring that reduces operations, so I'm submitting the baseline form again. With one ciphertext input and plaintext coefficients, x*A needs one plaintext multiply and one rescale, and B needs one plaintext add.

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