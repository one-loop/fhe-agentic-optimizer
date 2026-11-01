Attempts 4 and 5 were each about 2% faster than the best, short of the 3% needed. This attempt restructures the polynomial to trade plaintext multiplications and alignments for one extra ciphertext multiplication.

The idea is to factor z out of all odd terms, using T3 = z·W with W = 2·T2 − 1:

y = c0 + (c2 − c6)·T2 + T4·[(c4 + 2c6·T2) + 2z·(c5 + c7·W)] + z·(c1 − c7 + c3·W − c5·W)

What this changes compared with attempts 4–5:
- **T3 no longer built:** the coefficients go onto W at level 3 instead of onto T3, z·c1 or z·c7.
- **Plaintext multiplications:** 6 instead of 9.
- **Scale alignments:** about 3 instead of 5.
- **Ciphertext multiplications:** 5 instead of 4. The extra products (z·O, z·R, and T4 times the bracket) run at deep, cheaper levels.
- **Plaintext adds:** 3 more (c5, c1, c7 enter as adds).

My estimate is roughly break-even to slightly faster, so this is a gamble. It has not been run yet.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext T2 = zz + zz - one;
  Ciphertext W = T2 + T2 - one;
  Ciphertext T2s = square(T2);
  Ciphertext T4 = T2s + T2s - one;

  Ciphertext c6T2 = T2 * c6;
  Ciphertext E = (c6T2 + c6T2) + c4;
  Ciphertext O = W * c7 + c5;
  Ciphertext zO = z * O;
  Ciphertext high = T4 * (E + (zO + zO));

  Ciphertext R = ((W * c3 - W * c5) + c1) - c7;
  Ciphertext odd = z * R;

  Ciphertext even_low = T2 * c2 - c6T2;
  Ciphertext result = high + odd + even_low + c0;
  result.set_output("y");
}
```