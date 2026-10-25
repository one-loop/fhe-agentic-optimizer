I'm going back to attempt 4, since the early-c7 variant caused a scale mismatch. The multiplicative structure stays the same, but I remove two plaintext adds (about 2.5 ms each) using g5 = 2·T4 − g3 and g7 = 2·T6 − 2·T4 + g3. Those relations derive from the same identities, so they need no `one` plaintext.

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
  Plaintext c0("c0");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext s2 = square(t2);
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext m = t2 * t4;
  Ciphertext t6 = m + m - t2;
  Ciphertext g3 = t2 + t2 - one;
  Ciphertext g5 = t4 + t4 - g3;
  Ciphertext g7 = (t6 + t6) - (t4 + t4) + g3;
  Ciphertext q = g3 * c3;
  q = q + g5 * c5;
  q = q + g7 * c7;
  q = q + c1;
  Ciphertext result = z * q;
  result = result + c0;
  result.set_output("y");
}
```