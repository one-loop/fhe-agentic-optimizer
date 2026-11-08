The idea is to fold the T3 terms into products with T2, which removes one level. I use T3 = 2z·T2 − z, so the coefficient is applied to z first, at level 2, and then multiplied by T2.

**High part.**
q = c4 + 2(c5−c7)z + 2·T2·(c6 + 2c7z)

**Low part.**
low = c0 + (c1−c7)z − (c3−c5)z + T2·(2(c3−c5)z + c2 − c6)

**Constants without plaintext-plaintext arithmetic.** Each difference like c2 − c6 is formed as (ct + c2) − c6. Every operation still has a ciphertext operand.

**Cost compared with attempt 1.**

| | Attempt 1 | This kernel |
|---|---|---|
| Levels | 5 | 4 |
| ct-ct multiplications | 4 | 5 |
| ct-pt multiplications | 9 | 5 |

Every operation becomes cheaper because each ciphertext carries one prime fewer.

**Scale alignment.** Every same-level addition combines terms with identical multiplication history. Terms with different history meet at different levels, so no scale mismatch arises.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;            // L1
  Ciphertext zz = z * z;
  Ciphertext t2 = zz + zz - one;       // L2
  Ciphertext t2s = t2 * t2;
  Ciphertext t4 = t2s + t2s - one;     // L3

  Ciphertext a1 = z * c1;              // L2
  Ciphertext a3 = z * c3;
  Ciphertext a5 = z * c5;
  Ciphertext a7 = z * c7;

  // high part q = c4 + 2(c5-c7) z + 2 T2 (c6 + 2 c7 z)
  Ciphertext v = a7 + a7 + c6;         // L2
  Ciphertext hv = v * t2;              // L3
  Ciphertext d57 = a5 - a7;            // L2
  Ciphertext q = hv + hv + (d57 + d57 + c4);

  // low part: c0 + (c1-c7) z - g + T2 (2g + c2 - c6), g = (c3-c5) z
  Ciphertext g = a3 - a5;              // L2
  Ciphertext m = g + g + c2 - c6;      // L2
  Ciphertext lm = m * t2;              // L3
  Ciphertext low = lm + (a1 - a7 - g + c0);

  Ciphertext y = t4 * q + low;         // L4
  y.set_output("y");
}
```