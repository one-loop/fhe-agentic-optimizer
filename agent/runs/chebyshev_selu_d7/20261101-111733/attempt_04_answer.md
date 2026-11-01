The error came from adding cubic terms (scale D^7) to quadratic T2 terms (scale D^5) at level 3. I re-derived the Chebyshev sum so that the only level-3 terms that meet are of the same kind: a level-2 term z·(coefficients) multiplied by a level-2 quadratic.

The identities used are:
- T3 = z·u, with u = 2T2 − 1
- T5 = 2zT4 − T3
- T6 = 2T2T4 − T2
- T7 = 2zT6 − T5

With w = c6 + 2c7·z, d = c5·z − c7·z and g = c4 + 2d, the polynomial becomes:

y = c0 + c1·z + c2·T2 + T4·(2·T2·w + g) + [(c3·z − d)·u − T2·w]

Expected counts:
- 5 ciphertext multiplications: z², T2², T2·w, (c3·z − d)·u and the T4 product.
- 6 plaintext multiplications.
- 4 levels.

Scale checks:
- **The bracketed term:** (c3·z − d)·u and T2·w have the same multiplication history (D^7), so they can be added at level 3.
- **c2·T2:** it is D^5 at level 3. It is first added to c1·z (level 2) and then joined to the result at level 4, so it never meets a D^7 term at the same level.

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

  Ciphertext z = x * alpha;
  Ciphertext zz = z * z;
  Ciphertext T2 = zz + zz - one;
  Ciphertext u = T2 + T2 - one;
  Ciphertext t22 = T2 * T2;
  Ciphertext T4 = t22 + t22 - one;

  Ciphertext z1 = z * c1;
  Ciphertext z3 = z * c3;
  Ciphertext z5 = z * c5;
  Ciphertext z7 = z * c7;

  Ciphertext w = z7 + z7 + c6;
  Ciphertext d = z5 - z7;
  Ciphertext g = d + d + c4;
  Ciphertext combo = z3 - d;

  Ciphertext T2w = T2 * w;
  Ciphertext inner = T2w + T2w + g;
  Ciphertext high = T4 * inner;

  Ciphertext mid = combo * u - T2w;
  Ciphertext low = T2 * c2 + z1;

  Ciphertext result = high + mid;
  result = result + low;
  result = result + c0;
  result.set_output("y");
}
```