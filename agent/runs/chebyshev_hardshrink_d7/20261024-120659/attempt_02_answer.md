Idea: combine the T5 and T7 terms using T5 = 2·T2·T3 − z and T7 = 2·T3·T4 − z. Then c5·T5 + c7·T7 = T3·(2(c5·T2 + c7·T4)) − (c5+c7)·z, and c3·T3 folds into the same T3 product. This needs fewer ct-ct multiplies, and every addition joins terms at different levels, which avoids the scale mismatch from attempt 1.

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
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext u = t2 * c5 + t4 * c7;
  Ciphertext u2 = u + u;
  Ciphertext u3 = u2 + c3;
  Ciphertext r = t3 * u3;
  Ciphertext zs = z * c5 + z * c7;
  Ciphertext lin = z * c1 - zs;
  Ciphertext result = r + lin;
  result.set_output("y");
}
```