I have not found a faster circuit. Seven attempts, including this one, all came out as the baseline circuit or can't beat it. This final submission is the baseline form again: one ct-pt multiply with its rescale, plus one plaintext add. I see no way to remove any of those operations when A and B are arbitrary plaintext vectors.

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