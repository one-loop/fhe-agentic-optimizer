Idea: the polynomial uses only T1, T2, T4, T6 plus a constant, so it is even in z apart from the c1·z term. That means y = c0 + c1·z + P(z²), where P(u) = c2·T2 + c4·T4 + c6·T6 is a cubic in u = z².

T2 = 2u − 1
T4 = 8u² − 8u + 1
T6 = 32u³ − 48u² + 18u − 1

The coefficients arrive only as plaintext inputs, so I can't precompute combined constants. Plaintext-plaintext arithmetic is also disallowed, so every combination has to involve a ciphertext.

Operation plan:
- z = x·alpha (ct-pt mul, level 1).
- u = z·z (level 2).
- u2 = u·u (level 3).
- u3 = u2·u would be level 4.

Expanding in powers of u requires the plaintext combinations a1 = 2c2 − 8c4 + 18c6, a2 = 8c4 − 48c6 and a3 = 32c6. These cannot be formed from plaintexts alone, so I'd have to use T-basis terms with ciphertext operands instead.

Alternative: write P as a polynomial in T2 and use the identities T4 = 2T2² − 1 and T6 = 2·T2·T4 − T2. Let w = T2 = 2u − 1. Then:

P = c2·w + c4·(2w² − 1) + c6·(4w³ − 3w)

I'll use w with Horner-like grouping: P = w·(c2 + c4·2w + c6·(4w² − 3)) − c4. This still needs plaintext multiplications, but there are few of them.

Depth calculation:
- z: level 1.
- u = z², so w = u + u − one: level 2.
- w² = w·w: level 3.
- w³ = w²·w: level 4.
- Terms: c6·(w³+w³)+… would need constant multiplications by 4 and 3, which I can build by additions.

Cheaper grouping: P = c2·w + c4·(2w² − 1) + c6·(4w³ − 3w).
Let s = w² (level 3). Then 4w³ − 3w = w·(4s − 3), and 4s − 3 has no constant 3 available, only `one`. I can build it as s4 = 4s, minus 3·one, which needs ciphertext operands, so this doesn't work directly.

Simpler approach: compute q = c6·(4s) via additions, and have the rest as separate terms:
- A = (c2 − 3c6)·w is not formable, so use c2·w and c6·w separately.
- term_w = w·c2 and term_w6 = w·c6.
- Then c6·4w³ = (w·c6)·(s+s+s+s) is a ct-ct multiplication of a level-2 value by a level-3 value, giving level 4.

That saves a bit compared with the baseline's depth of 7 and its 5 ct-ct multiplications, but total depth would be 4 here, which is still fine.

Final plan:
- z = x·alpha
- u = z·z, w = 2u − one
- s = w·w
- m = w·c6 (level 3)
- t = (s·4) − ... I'll use 4s via additions, then multiply by m for the cubic term.
- result = c1·z + c2·w + c4·(2s − one) + (4s·m − 3m) + c0

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
  Ciphertext w = u + u - one;
  Ciphertext s = w * w;
  Ciphertext s2 = s + s;
  Ciphertext s4 = s2 + s2;
  Ciphertext m = w * c6;
  Ciphertext m3 = m + m + m;
  Ciphertext cubic = s4 * m - m3;
  Ciphertext t4 = s2 - one;
  Ciphertext r = cubic + t4 * c4;
  r = r + w * c2;
  r = r + z * c1;
  r = r + c0;
  r.set_output("y");
}
```