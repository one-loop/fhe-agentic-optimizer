I can't find a cheaper circuit. The function needs one plaintext multiply, which brings one rescale with it, and one plaintext add, and each of those is already at its minimum. The numeric constants and plaintext-plaintext arithmetic that a cheaper rewrite would need are not allowed in this DSL. The one difference that did appear in attempt 3 (the negation form) was measurement noise, not a real saving.

I'm resubmitting the minimal form, and I expect it to be reported as identical to the baseline.

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