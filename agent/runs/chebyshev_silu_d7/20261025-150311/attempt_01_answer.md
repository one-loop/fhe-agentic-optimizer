Idea: write the polynomial in the power basis, which needs fewer multiplications. The needed terms are c1·z + c2·T2 + c4·T4 + c6·T6 + c0, which is an even polynomial plus c1·z. In u = z², T2 = 2u−1, T4 = 8u²−8u+1, and T6 = 32u³−48u²+18u−1. So y = c1·z + A·u + B·u² + C·u³ + D, where the coefficients are combinations of c's. Those combinations need plaintext-plaintext arithmetic, which is not allowed, so I have to apply the c's to ciphertexts instead.

Cheap evaluation: compute u = z² and u2 = u², then the even part is E = c0 + c2·T2 + c4·T4 + c6·T6.

Using the T-recurrences on ciphertexts:
- z = x·alpha (plaintext mult, 1 level).
- T2 = 2u − one (u = z·z, level 2).
- T4 = 2·T2² − one (level 3).
- T6 = 2·T2·T4 − T2 (level 4).
- Then multiply each of T2, T4, T6 by its coefficient (one more level).

That is 4 ct-ct multiplications (z², T2², T2·T4, and nothing else), so 3 ct-ct multiplications instead of 5. Depth is 4 for T6, plus 1 for the coefficient, so 5 levels.

To save a level, I can fold alpha in. Computing z² as (x·x)·(alpha²) would need plaintext-plaintext arithmetic, so instead I use z = x·alpha and u = z·z; there is no cheaper way here.

Terms:
- c1·z is at level 2 (z·c1).
- c2·T2 is at level 3.
- c4·T4 is at level 4.
- c6·T6 is at level 5.

The sum meets terms at different levels, which CHEHAB aligns.

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
  Ciphertext s2 = square(t2);
  Ciphertext t4 = s2 + s2 - one;
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