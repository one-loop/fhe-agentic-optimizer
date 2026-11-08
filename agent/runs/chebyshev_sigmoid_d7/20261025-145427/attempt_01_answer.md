Idea: the polynomial is odd apart from c0, so write p(z) = c0 + z·(a1 + a3 z² + a5 z⁴ + a7 z⁶) with u = z², where each a_k is a power-basis coefficient. The power-basis coefficients must come from plaintext inputs without plaintext-plaintext arithmetic, so I stay in the Chebyshev basis and fold the constants into ciphertext-plaintext multiplies.

T3 = 4z³ − 3z, T5 = 16z⁵ − 20z³ + 5z, T7 = 64z⁷ − 112z⁵ + 56z³ − 7z. Numeric constants such as 4 or 3 are not allowed, and plaintext-plaintext arithmetic is not allowed either. So the coefficients combine only through ciphertext operations.

Alternative: use T_{2k} identities to cut depth.
- u = z² (one square).
- T3 = z(2u−1)·2 − z, which is z·(4u−3). Only doubling by addition is available, so I build 4u−3 from u+u, doubling again, and subtract (one+one+one). That needs `one` as a plaintext, and a plaintext-plaintext sum is not allowed, so I subtract `one` three times from a ciphertext instead.
- Write p(z) = c0 + z·Q(u), where Q(u) = c1 + c3(4u−3) + c5(16u²−20u+5) + c7(64u³−112u²+56u−7).
- Q needs c-weighted terms. With w = T3/z-style polynomials in u: P3 = 4u−3, P5 = 16u²−20u+5, P7 = 64u³−112u²+56u−7. Compute u² = u*u and u³ = u²*u, then the sum would require numeric multiples such as 16, which need repeated doubling (cheap adds).
- To avoid the large constants, I use the recurrence on the ciphertext side: a = 2u−1 = T2. Then T4 = 2a²−1, and so on. This is more multiplications.

I'll go with the direct approach, using adds for the integer multiples.
Let u = z², v = u² (depth 2), w = u·v (depth 3). Then
Q = c1 + c3·(4u−3) + c5·(16v−20u+5) + c7·(64w−112v+56u−7).
Group by powers: Q = (c1 − 3c3 + 5c5 − 7c7) + u(4c3 − 20c5 + 56c7) + v(16c5 − 112c7) + 64c7·w.
Forming the coefficient combinations needs plaintext arithmetic, so instead I multiply each Chebyshev term by its c_k as plaintext times ciphertext and build the integer multiples by doubling the ciphertext. That is, A = c3·u, then 4A = A doubled twice, and so on. This costs more plaintext multiplies but they are cheap. Terms: c3·u, c5·u, c7·u, c5·v, c7·v, c7·w, c3, c5, c7 and c1 constants. This is more messy, so I take the simpler option: compute the c_k·T_k terms from T_k built via cheap ciphertext operations, with fewer ct-ct multiplies, using T-products:
T2 = 2z²−1, T3 = 2·z·T2 − z, T4 = 2T2²−1, T5 = 2·z·T4 − T3 (z·T4 or T2·T3 − z), T7 = 2·T3·T4 − z.
That is: z², z·T2, T2², z·T4, T3·T4, a total of 5 ct-ct multiplies (versus 6), with depth ≤ 4 and a lower level at the end.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c3("c3"), c5("c5"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext t2 = s + s - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext t3t4 = t3 * t4;
  Ciphertext t7 = t3t4 + t3t4 - z;
  Ciphertext zt4 = z * t4;
  Ciphertext t5 = zt4 + zt4 - t3;
  Ciphertext result = z * c1 + t3 * c3 + t5 * c5 + t7 * c7 + c0;
  result.set_output("y");
}
```