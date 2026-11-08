I don't see a cheaper circuit. Any exact evaluation of x*A + B needs one ct-pt multiply with its rescale, plus one plaintext add. I'm resubmitting the same form instead of a variant that adds operations.

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