The minimum is still one plaintext multiply plus one plaintext add. This attempt puts the negation after the subtraction, so the operation counts differ from earlier variants, but I don't expect it to beat the baseline.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext nx = -x;
  Ciphertext t = nx * a;
  Ciphertext u = t - b;
  Ciphertext y = -u;
  y.set_output("y");
}
```