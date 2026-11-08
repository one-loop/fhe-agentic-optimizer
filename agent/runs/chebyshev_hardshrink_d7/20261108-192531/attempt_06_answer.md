This is a regrouping of attempt 5. Substituting U = 2a·T2 + ea into the −2U correction gives:

y = 4·T2²·U + 2·T2·(d − 2a) + (lin − 2·ea)

The second product, (d − 2a)·T2, replaces d·T2, so there are still 5 ciphertext and 5 plaintext multiplications. The level-2 linear part (lin − 2·ea) is now a single ciphertext added to the level-3 product. This saves a few additions and may let CHEHAB reuse the aligned ea.

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

  Ciphertext z = x * alpha;              // L1
  Ciphertext zz = z * z;                 // L2
  Ciphertext t2 = zz + zz - one;         // L2
  Ciphertext t2s = t2 * t2;              // L3  T2^2

  Ciphertext a = z * c7;                 // L2
  Ciphertext e = z * c5;                 // L2
  Ciphertext d = z * c3 - e;             // L2  (c3-c5) z
  Ciphertext f = z * c1 - a;             // L2  (c1-c7) z
  Ciphertext lin = f - d;                // L2
  Ciphertext ea = e - a;                 // L2
  Ciphertext a2 = a + a;
  Ciphertext g = d - a2;                 // L2
  Ciphertext ea2 = ea + ea;
  Ciphertext R = lin - ea2;              // L2

  Ciphertext at2 = a * t2;               // L3
  Ciphertext at2x2 = at2 + at2;
  Ciphertext U = at2x2 + ea;             // L3  c5 z + c7 T3
  Ciphertext gt2 = g * t2;               // L3
  Ciphertext P = gt2 + gt2;              // L3
  Ciphertext S = P + R;                  // L3

  Ciphertext m = t2s * U;                // L4
  Ciphertext m2 = m + m;
  Ciphertext m4 = m2 + m2;
  Ciphertext y = m4 + S;
  y.set_output("y");
}
```