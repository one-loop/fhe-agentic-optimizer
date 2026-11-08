I have nothing new to try. The baseline is already minimal, and resubmitting it gains nothing, so I'm stopping here.

The best kernel remains the baseline form: `y = x * A + B`, with one plaintext multiply and rescale, one plaintext add, and 1 level consumed. It runs in about 17.90 ms.

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