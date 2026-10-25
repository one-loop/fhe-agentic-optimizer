Idea: keep the same Paterson-Stockmeyer-style structure, since it already needs only 4 ct-ct multiplies. This attempt removes a redundant plaintext multiply by reusing `a6 = T2*c6` for the subtracted `c6*T2` term. It also adds the same-level terms together before they meet the deeper product, so CHEHAB needs fewer level and scale alignments.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;
  Ciphertext s2 = T2 * T2;
  Ciphertext T4 = s2 + s2 - one;

  Ciphertext a5 = z * c5;
  Ciphertext a6 = T2 * c6;
  Ciphertext a7 = T3 * c7;
  Ciphertext inner = a5 + a6 + a7;
  Ciphertext Q = inner + inner + c4;
  Ciphertext P = T4 * Q;

  Ciphertext zt = z * c1 - z * c7;
  Ciphertext t2t = T2 * c2 - a6;
  Ciphertext t3t = T3 * c3 - T3 * c5;
  Ciphertext low = (zt + t2t) + c0;
  Ciphertext r = P + low + t3t;
  r.set_output("y");
}
```