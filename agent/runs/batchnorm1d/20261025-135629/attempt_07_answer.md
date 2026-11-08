I don't see a cheaper circuit. The baseline already has one plaintext multiply, one rescale and one plaintext add. Removing any of them would need plaintext-plaintext arithmetic such as B/A, which CKKS rules forbid, or extra ciphertext operations. I'm resubmitting the minimal form.

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