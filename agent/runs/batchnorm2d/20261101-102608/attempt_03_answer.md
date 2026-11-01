I still have no cheaper circuit, so I am submitting the baseline a third time. The judge will mark it identical again, and it cannot become a new best.

The reason is unchanged from my last answer. The baseline (one plaintext multiply, one rescale, one plaintext add) is the minimum for `x·A + B`. Every other circuit I can write adds operations and would be slower.

I'd suggest stopping here and keeping the baseline as the final result.

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