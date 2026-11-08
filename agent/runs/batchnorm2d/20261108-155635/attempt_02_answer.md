I can't remove the multiply, so this attempt changes how the shift is applied. I subtract a negated copy of `B` instead of adding `B`. That may or may not compile, since the negation is plaintext-only arithmetic, but it would at least be a different circuit.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext y = t - (-b);
  y.set_output("y");
}
```