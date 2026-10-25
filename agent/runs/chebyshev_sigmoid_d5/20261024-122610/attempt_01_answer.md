Idea: write the odd polynomial in z directly: y = c0 + z·(c1 + c3·(4z²−3) + c5·(16z⁴−20z²+5)).
With u = z², this is z·(a + b·u + d·u²), where the coefficients are combined from the plaintext inputs. Plaintext-plaintext arithmetic is not allowed, so I can't precompute the combined coefficients. Instead I expand the polynomial in terms of the ciphertext:

y = c0 + c1·z + c3·(4z³−3z) + c5·(16z⁵−20z³+5z)
 = c0 + z·c1' ... which still needs the combined coefficients.

Instead, apply the coefficients to ciphertext powers:
- P1 = z·(c1 − 3c3 + 5c5), etc. needs constants. That fails, so I use the T_k form but cut the multiplications.

Plan:
- z = x·alpha (one ct-pt multiply).
- u = z² (one ct-ct multiply).
- T3 = 4z³ − 3z = z·(4u − 3); T5 = z·(16u² − 20u + 5).
- Without constants, "4u" is built by additions: u2 = u+u, u4 = u2+u2. The constant 3 can't be made without `one` (one is a plaintext, so `+ one` is allowed as ct+pt).
- Let w = 4u − 3·one, which is u4 − one − one − one (ct − pt, allowed).
- T3 = z·w.
- T5 = 2·u2'·… I'll use T5 = 2z·T4 − T3 with T4 = 2·(2u−1)² − 1, and T2 = 2u − 1.
- Instead use T5 = z·(16u² − 20u + 5), with v = 4u − 5/2·… which again needs constants. Rather, I keep T4 = 2·T2² − 1 and T5 = 2·z·T4 − T3.

Multiplications: z·z, T2², z·T4, z·T2 (T3 = 2z·T2 − z). That is 4 ct-ct multiplications, the same as the baseline.

Reduce depth by grouping the coefficients: result = c1·z + c3·T3 + c5·T5 + c0. Better to fold the coefficients into the multiplications:
- T3 = 2·z·T2 − z, so c3·T3 = (z·c3)·(2T2) − c3·z. Here z·c3 is a ct-pt multiply that runs in parallel with computing T2.
- T5 = 2·z·T4 − T3, so c5·T5 = (z·c5)·(2T4) − c5·T3.

Simpler is the baseline structure with fewer levels. I take the following form:
- z = x·alpha.
- u = z·z.
- A = z·c3 and B = z·c5 (parallel, level 1).
- T2' = (u+u) − one (level 2).
- T3c = A·(T2'+T2') − A·… (A·2T2 = 2c3·z·T2, and c3·z is just A).
- T4 = 2·T2'² − one. Depth 3.
- B·(2T4) has depth 4.
- c5·T3 requires T3c5 = c5·T3, which is another multiplication, and the cost grows.

I use the simple version and keep it correct:

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t4s = t2 * t2;
  Ciphertext t4 = t4s + t4s - one;
  Ciphertext zt4 = z * t4;
  Ciphertext t5 = zt4 + zt4 - t3;
  Ciphertext r = z * c1 + t3 * c3 + t5 * c5 + c0;
  r.set_output("y");
}
```