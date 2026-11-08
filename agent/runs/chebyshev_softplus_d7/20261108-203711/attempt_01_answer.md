The baseline builds every Chebyshev term up to T7 with a ladder of 5 ciphertext multiplications and uses 7 levels. Only the even terms and T1 are needed, so I compute them from T2 using the doubling identities:

- T2 = 2z² − 1
- T4 = 2·T2² − 1
- T6 = 2·T2·T4 − T2

I then factor the top two terms so T6 never has to be computed:

c4·T4 + c6·T6 = T4·(2·c6·T2 + c4) − c6·T2

with u = c6·T2 computed once and reused.

**Expected counts:**
- 3 ciphertext-ciphertext multiplications (z², T2², T4·inner).
- 4 plaintext multiplications (alpha, c1, c2, c6).
- 5 levels instead of 7.

**Scale check:** wherever two ciphertexts are added at the same level, they come from the same multiplication pattern. Otherwise they meet at different levels, which CHEHAB aligns for free.

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
  Ciphertext zz = z * z;               // level 2
  Ciphertext t2 = zz + zz - one;       // level 2
  Ciphertext t2s = t2 * t2;            // level 3
  Ciphertext t4 = t2s + t2s - one;     // level 3

  Ciphertext u = t2 * c6;              // level 3
  Ciphertext inner = u + u + c4;       // level 3
  Ciphertext high = t4 * inner - u;    // level 4: c4*T4 + c6*T6

  Ciphertext term1 = z * c1;           // level 2
  Ciphertext term2 = t2 * c2;          // level 3

  Ciphertext result = high + term2 + term1 + c0;
  result.set_output("y");
}
```