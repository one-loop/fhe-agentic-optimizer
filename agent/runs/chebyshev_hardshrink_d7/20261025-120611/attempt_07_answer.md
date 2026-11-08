This attempt removes two of the four plaintext adds in the constant term. The constant −c5+c7 equals −(c5−c7), and the ciphertext `s = 2·s0 + c5 − c7` already contains that quantity. So −c5+c7 can be recovered from existing ciphertexts as `s2 − s` (where `s2 = 2·s0`), which costs two ciphertext adds (~0.5 ms each) instead of two plaintext adds (~2.5 ms each).

The result is p = 2(tc3−tc5) − s2 − s + 4q + c1 − c3, which equals the previous p.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext t = zz + zz - one;
  Ciphertext t2 = square(t);
  Ciphertext s0 = t * c7;
  Ciphertext s2 = s0 + s0;
  Ciphertext s = s2 + c5 - c7;
  Ciphertext q = s * t2;
  Ciphertext q2 = q + q;
  Ciphertext q4 = q2 + q2;
  Ciphertext tc3 = t * c3;
  Ciphertext tc5 = t * c5;
  Ciphertext dd = tc3 - tc5;
  Ciphertext p = dd + dd + q4 - s2 - s;
  p = p + c1 - c3;
  Ciphertext y = z * p;
  y.set_output("y");
}
```