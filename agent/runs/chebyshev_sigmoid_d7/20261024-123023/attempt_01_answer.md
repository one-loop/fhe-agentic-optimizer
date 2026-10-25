Idea: the polynomial is odd, so y = c0 + c1 z + c3 z^3 + c5 z^5 + c7 z^7 in monomial form, i.e. y = c0 + z·P(z²) with P(u) = a1 + a3 u + a5 u² + a7 u³. The monomial coefficients cannot be computed from the plaintext inputs without plaintext-plaintext arithmetic, so I keep the Chebyshev form and fold the coefficients in ahead of time.

Using the Chebyshev identities:
- T3 = 4z³ − 3z
- T5 = 16z⁵ − 20z³ + 5z
- T7 = 64z⁷ − 112z⁵ + 56z³ − 7z

Constants such as 4 and 3 can only be made by additions, and combining the coefficients needs plaintext arithmetic, so I avoid that and use a different structure.

Plan: let w = x·alpha and u = w² (one ct-ct multiply). Then T3 = z(2u−1)·2 − z = z(4u−3), and in general T_odd = z·Q_k(u), so
y = c0 + z·(c1 + c3(4u−3) + c5(16u²−20u+5) + c7(64u³−112u²+56u−7)).
Taking the inner polynomial as a function of u requires the plaintext-scalar combinations of the coefficients, which I cannot form. So I evaluate it with ct×pt multiplies instead:
- T3' = 4u − 3 = (u+u+u+u) − (one+one+one), but constants must come from `one` and need a ciphertext operand, so 3·one cannot be formed directly.
- Instead, write S = c1 + c3·T3' + c5·T5' + c7·T7', where T3' = z-free polynomial in u. Each T_k' is a ciphertext in u built by adds and subtractions of ciphertext terms and plaintext `one` (plaintext-add is allowed).

Let u2 = u·u (one ct-ct multiply), and u3 = u2·u (one ct-ct multiply, depth 3). A shallower option: e = u2 and u·(stuff). With the monomials u, u2, u3 the coefficients would be combinations of c's, which I cannot form. So I keep the T3', T5', T7' as ciphertexts:
- T3' = 4u − 3: only adds and plaintext subtractions of `one`.
- T5' = 16u² − 20u + 5 = T3'·(4u−3)... I need T5' via the recurrence on u, which costs a multiply each.

That would cost about the same as the baseline, so I settle for a moderate saving: compute z once, u = z², T3 = z·T3', and then T5 and T7 through products of lower-order terms, using T5 = 2·T2·T3 − T1 and T7 = 2·T3·T4 − T1, with T4 = 2T2² − 1.

Depth: z (1), T2 (2), T3 = 2 z T2 − z (3), T4 (3), T5 = 2 T2 T3 − z (4), T7 = 2 T3 T4 − z (4). Depth drops from 8 to about 5, and it needs 6 ct-ct multiplies. The coefficient multiplies are at the end, so the total depth is 5.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext z_t2 = z * t2;
  Ciphertext t3 = z_t2 + z_t2 - z;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext p5 = t2 * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext p7 = t3 * t4;
  Ciphertext t7 = p7 + p7 - z;
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Plaintext c0("c0");
  Ciphertext result = z * c1;
  result = result + t3 * c3;
  result = result + t5 * c5;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
```