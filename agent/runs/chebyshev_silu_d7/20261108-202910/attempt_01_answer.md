The baseline builds every Chebyshev term up to T7 by recurrence, which takes 7 levels. Only the even terms and c1·z are needed, so I build them from T2 using the doubling identities:

- T4 = 2·T2² − 1
- T6 = 2·T2·T4 − T2

Then I factor the top two terms to save a multiplication:

c4·T4 + c6·T6 = T4·(c4 + 2·c6·T2) − c6·T2

This gives:

y = T4·(2·c6·T2 + c4) + c2·T2 − c6·T2 + c1·z + c0

**Expected cost (not yet run):** 3 ciphertext multiplications (z², T2², and T4·w) and 4 plaintext multiplications (alpha, c1, c2, c6). Multiplicative depth should drop from 7 to 4. The terms added at the same level come from the same multiplication pattern, so there should be no scale mismatch.

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

  Ciphertext z = x * alpha;            // level 1
  Ciphertext u = z * z;                // level 2
  Ciphertext t2 = u + u - one;         // level 2
  Ciphertext s = t2 * t2;              // level 3
  Ciphertext t4 = s + s - one;         // level 3

  Ciphertext c6t2 = t2 * c6;           // level 3
  Ciphertext w = c6t2 + c6t2 + c4;     // level 3
  Ciphertext high = t4 * w;            // level 4: c4*T4 + c6*T6 + c6*T2

  Ciphertext c2t2 = t2 * c2;           // level 3
  Ciphertext mid = c2t2 - c6t2;        // level 3

  Ciphertext lin = z * c1;             // level 2

  Ciphertext result = high + mid + lin + c0;
  result.set_output("y");
}
```