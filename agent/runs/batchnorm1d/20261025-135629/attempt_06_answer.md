Attempt 5 was my own algebra mistake: -(b - t) equals t - b, not t + b. The corrected form is b - (-t) = b + t.

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