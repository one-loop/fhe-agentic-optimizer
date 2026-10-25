I could not find a circuit cheaper than attempt 1, so I'm resubmitting that kernel unchanged. Every alternative needs a third plaintext multiplication or an extra level. This form uses 1 ciphertext-ciphertext multiplication and 2 plaintext multiplications, each applied once.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext m = z * c2;
  Ciphertext m2 = m + m;
  Ciphertext s = m2 + c1;
  Ciphertext p = s * z;
  Ciphertext r = p + c0;
  Ciphertext y = r - c2;
  y.set_output("y");
}
```