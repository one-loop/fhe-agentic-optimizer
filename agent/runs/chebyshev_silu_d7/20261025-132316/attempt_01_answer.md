Idea: the output is a polynomial in z with only even terms c2, c4, c6 plus the odd term c1 (c3, c5, c7 are dropped). Rather than building the full Chebyshev recurrence, I expand it in the monomial basis, using u = z².

- T2 = 2u − 1
- T4 = 8u² − 8u + 1
- T6 = 32u³ − 48u² + 18u − 1
- y = c0 + c1 z + c2 T2 + c4 T4 + c6 T6 = (c0 − c2 + c4 − c6) + c1 z + (2c2 − 8c4 + 18c6) u + (8c4 − 48c6) u² + 32 c6 u³

Plaintext-plaintext arithmetic is not allowed, and constants cannot be written inline. So I fold the coefficients into the ciphertext side, with these combined coefficients as plaintext multiplications:

- Let A = 2c2 − 8c4 + 18c6, B = 8c4 − 48c6, C = 32c6. These need plaintext-plaintext ops, which are forbidden, so they cannot be formed directly.

Instead I use the Chebyshev form with few multiplications:
- u = z², w = u·u (T4-like), and T2 = 2u − 1.
- T4 = 2·T2² − 1 and T6 = 2·T2·T4 − T2.
- y = c0 + c1 z + c2 T2 + c4 T4 + c6 T6.
- T6 term: c6·(2·T2·T4 − T2) = (T2·(2c6))... doubling a plaintext is not allowed either. Instead use T2·T4 and the sum (p+p) at the ciphertext level.

Plan:
1. z = x·alpha (ct-pt mul, level 1).
2. u = z² (level 2). T2 = u + u − one.
3. T4 = 2·T2² − 1 (level 3).
4. P = T2·T4 (level 4). T6 = P + P − T2.
5. Terms: c1·z, c2·T2, c4·T4, c6·T6. The T6 term costs a multiplication at level 5.

Better: c6·T6 = c6·(2P − T2) = (c6·P)·2 − c6·T2. To cut depth, I can fold the T2 and T4 pieces into one multiply: let Q = c4·T4 − c6·T2 + ... but this is still depth 4–5. I'll accept that: this uses 4 ct-ct muls (u, T2², P, plus the z multiply as ct-pt) versus 5 in the baseline, and fewer rescales and alignments.

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
  Ciphertext u = square(z);
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(t2);
  Ciphertext t4 = s + s - one;
  Ciphertext p = t2 * t4;
  Ciphertext t6 = p + p - t2;
  Ciphertext r = z * c1;
  r = r + t2 * c2;
  r = r + t4 * c4;
  r = r + t6 * c6;
  r = r + c0;
  r.set_output("y");
}
```