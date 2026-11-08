I have no cheaper circuit to propose. The baseline's one ct-pt multiply, one rescale and one plaintext add look like the minimum for y = x*A + B with A and B as arbitrary plaintext vectors. Every rewrite I have submitted normalized to that same circuit, and anything structurally different would add operations. I am resubmitting the baseline form.

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