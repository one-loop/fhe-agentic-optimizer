Idea: convert to a monomial basis in z and evaluate with fewer multiplications. The coefficients are only available as plaintext inputs, so the conversion has to be done with ciphertext-plaintext ops, which are cheap compared to ct-ct multiplies.

Chebyshev to power basis, with u = z²:
- T2 = 2u − 1
- T3 = 4z³ − 3z
- T4 = 8u² − 8u + 1
- T5 = 16z⁵ − 20z³ + 5z
- T6 = 32u³ − 48u² + 18u − 1
- T7 = 64z⁷ − 112z⁵ + 56z³ − 7z

Numeric constants such as 4 or 3 can't be written directly, so the power-basis coefficients would have to be built from the plaintext inputs. That needs plaintext-plaintext arithmetic, which isn't allowed. A different route avoids this.

Split into odd and even parts: y = E(u) + z·O(u), where u = z².
- E = c0 + c2T2 + c4T4 + c6T6, a cubic in u.
- O = (c1 + c3T3/z + …), also a cubic in u.

The coefficient combinations can't be formed without plaintext arithmetic, so I keep the Chebyshev structure and use the doubling identities:
- T2 = 2u − 1, with u = z·z.
- T4 = 2T2² − 1.
- T3 = 2·z·T2 − z.
- T6 = 2T3² − 1.
- T5 = 2T3·T2 − z.
- T7 = 2T4·T3 − z.

Depths with z at depth 1 (after multiplying by alpha):
- T2 is at depth 2, T3 at 3, T4 at 3, T5 at 4, T6 at 4, T7 at 4.
- The coefficient multiplications add one more level, so the total is 5 levels instead of 8, which makes later operations cheaper.

Ct-ct multiplies: z² (1), z·T2 (2), T2² (3), T3² (4), T3·T2 (5), T4·T3 (6). That is still 6, but the depth is lower, so each multiply is cheaper.

I can't write "2·a − 1" with constants, but I can write a+a−one, using the `one` plaintext. For subtracting z, I use the ciphertext z directly. The scale and level alignment is handled automatically.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = t2 * t2;
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext t3t2 = t3 * t2;
  Ciphertext t5 = t3t2 + t3t2 - z;
  Ciphertext t3s = t3 * t3;
  Ciphertext t6 = t3s + t3s - one;
  Ciphertext t4t3 = t4 * t3;
  Ciphertext t7 = t4t3 + t4t3 - z;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext r = z * c1;
  r = r + t2 * c2;
  r = r + t3 * c3;
  r = r + t4 * c4;
  r = r + t5 * c5;
  r = r + t6 * c6;
  r = r + t7 * c7;
  r = r + c0;
  r.set_output("y");
}
```