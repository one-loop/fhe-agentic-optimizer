I have nothing new to propose. The circuit is already the minimum for y = x*A + B: one ct-pt multiply with its rescale, and one plaintext add. Alternatives such as a zero ciphertext, negated terms or reordered operands either collapse to this circuit or add operations. I'm keeping the baseline kernel.

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