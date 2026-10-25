Idea: the polynomial uses only c0, c1, c2, c4, c6, so I convert it to a polynomial in u = z² and cut the number of multiplications.

With z = x·alpha:
- T2 = 2u − 1
- T4 = 8u² − 8u + 1
- T6 = 32u³ − 48u² + 18u − 1

The result is y = c0 + c1·z + c2·T2 + c4·T4 + c6·T6. Collecting powers of u gives:
- A (constant) = c0 − c2 + c4 − c6
- B (u coefficient) = 2c2 − 8c4 + 18c6
- C (u² coefficient) = 8c4 − 48c6
- D (u³ coefficient) = 32c6

Computing these coefficients would be plaintext-plaintext arithmetic, which is not allowed, so I avoid it. Instead I evaluate with ciphertext-side operations.

Writing P(u) = A + B·u + C·u² + D·u³ as c-terms times T-polynomials keeps every multiplication involving plaintexts at ct-pt cost. The tradeoff is:
- u = z·z, one ct-ct multiplication.
- u2 = u·u, one ct-ct multiplication.
- T6 = T2·T4-style products would need extra ct-ct multiplications, so I avoid them.
- Computing T4 = 2·T2² − 1 gives T4 from T2 with one multiplication: T2 = 2u − 1, T4 = 2·T2² − 1.
- T6 = 2·T2·T4 − T2 needs one more multiplication.

That gives a ct-ct multiplication count of 4 (u, T2², T2·T4, plus z = x·alpha as a ct-pt multiplication), compared with 5 in the baseline. The depth is: z at level 1, u at 2, T2 at 2, T4 at 3, T6 at 4. Depth drops from 7 to about 5, and the later operations run at deeper levels, so they are cheaper.

The terms are then combined as c1·z + c2·T2 + c4·T4 + c6·T6 + c0, each with one ct-pt multiplication. CHEHAB aligns levels automatically.

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
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext m = t2 * t4;
  Ciphertext t6 = m + m - t2;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + t4 * c4;
  result = result + t6 * c6;
  result = result + c0;
  result.set_output("y");
}
```