The failure was a scale mismatch: the c5·w terms and the (c5·u)·w term landed at the same level with different multiplication histories. I now fold the whole z⁵ part into one ciphertext before applying c5: e5 = 5z − 20z³ + 16·z²·z³, and then multiply by c5 once. That keeps 3 ct-ct multiplies and puts the terms at different levels.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext w = z * u;
  Ciphertext uw = u * w;
  Ciphertext a2 = uw + uw;
  Ciphertext a4 = a2 + a2;
  Ciphertext a8 = a4 + a4;
  Ciphertext a16 = a8 + a8;
  Ciphertext w2 = w + w;
  Ciphertext w4 = w2 + w2;
  Ciphertext w8 = w4 + w4;
  Ciphertext w16 = w8 + w8;
  Ciphertext w20 = w16 + w4;
  Ciphertext z2 = z + z;
  Ciphertext z4 = z2 + z2;
  Ciphertext z3x = z4 - z;
  Ciphertext z5 = z4 + z;
  Ciphertext e3 = w4 - z3x;
  Ciphertext e5 = a16 + z5 - w20;
  Ciphertext r = z * c1 + e3 * c3 + e5 * c5 + c0;
  r.set_output("y");
}
```