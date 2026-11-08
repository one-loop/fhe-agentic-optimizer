The baseline is already at the arithmetic minimum: one plaintext multiply, one rescale and one plaintext add. I can't fold B into the multiply, because that would need B/A, which is plaintext arithmetic. This attempt keeps the same operations but writes the final step as a plaintext-minus-ciphertext subtraction followed by a negation, giving a slightly different structure. It is unlikely to beat the baseline.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext y = -(b - t);
  y.set_output("y");
}
```