Attempt 2 added extra multiplications, so it was slower. This attempt tries a different structure: the square form y = c2·(2z² − 1) + c1·z + c0. It uses `square(z)` and no extra ciphertext-ciphertext product. It needs 3 plaintext multiplications, so it may not beat attempt 1, but it tests a different circuit shape.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t = z2 + z2;
  Ciphertext r = t * c2;
  Ciphertext a = z * c1;
  r = r + a;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
```