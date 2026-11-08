This is one more structurally different form: negate the product instead of the input, then subtract it from `B`. It is still the same underlying work plus a negation, so I don't expect it to be faster than the baseline.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext n = -t;
  Ciphertext y = b - n;
  y.set_output("y");
}
```