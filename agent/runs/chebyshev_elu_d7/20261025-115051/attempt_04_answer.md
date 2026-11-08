Every term c_k·T_k has to sit at a distinct level, or the terms need identical multiplication histories. That forces a depth of 8, which the baseline already has. So this attempt keeps the depth-8 floor but pre-multiplies the coefficients into z. Each term becomes 2·(c_k·z)·T_{k−1} − c_k·T_{k−2}. Each term then ends one level shallower, so most levels stay distinct. The pair c2/c3 may still collide at level 3, which is a gamble.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext u = t2 + t2 - one;
  Ciphertext t3 = z * u;
  Ciphertext t4 = (z * t3) ;
  t4 = t4 + t4 - t2;
  Ciphertext t5 = z * t4;
  t5 = t5 + t5 - t3;
  Ciphertext t6 = z * t5;
  t6 = t6 + t6 - t4;

  Ciphertext cz1 = z * c1;
  Ciphertext cz3 = z * c3;
  Ciphertext cz4 = z * c4;
  Ciphertext cz5 = z * c5;
  Ciphertext cz6 = z * c6;
  Ciphertext cz7 = z * c7;
  Ciphertext cz2 = z * c2;

  Ciphertext r2 = (cz2 + cz2) * z - c2;
  Ciphertext r3 = (cz3 + cz3) * t2 - cz3;
  Ciphertext r4 = (cz4 + cz4) * t3 - c4 * t2;
  Ciphertext r5 = (cz5 + cz5) * t4 - c5 * t3;
  Ciphertext r6 = (cz6 + cz6) * t5 - c6 * t4;
  Ciphertext r7 = (cz7 + cz7) * t6 - c7 * t5;

  Ciphertext result = cz1 + r2;
  result = result + r3;
  result = result + r4;
  result = result + r5;
  result = result + r6;
  result = result + r7;
  result = result + c0;
  result.set_output("y");
}
```