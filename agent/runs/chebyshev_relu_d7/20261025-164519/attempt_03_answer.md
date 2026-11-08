The attempt 2 error was algebraic: I multiplied by s only twice, but the cubic needs three. The correct Horner form is s·(c2 − 3c6 + s·(2c4 + 4c6·s)), which adds one more ct-ct multiply (3 in total) and keeps depth 5.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext s = u + u - one;
  Ciphertext w = s * c6;
  Ciphertext w2 = w + w;
  Ciphertext w4 = w2 + w2;
  Ciphertext inner = w4 + c4;
  inner = inner + c4;
  Ciphertext m = s * inner;
  m = m + c2;
  m = m - c6;
  m = m - c6;
  m = m - c6;
  Ciphertext r = s * m;
  r = r + z * c1;
  r = r + c0;
  r = r - c4;
  r.set_output("y");
}
```