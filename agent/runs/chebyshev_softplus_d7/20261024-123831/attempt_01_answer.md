Idea: the polynomial uses only T1, T2, T4, T6 plus c0. Written in z², it is an even polynomial in z plus a linear term: c1·z + c2·T2 + c4·T4 + c6·T6 + c0 = P(z²) + c1·z, where P is a cubic in u = z².

Expand with T2 = 2u−1, T4 = 8u²−8u+1, T6 = 32u³−48u²+18u−1. The coefficients of u^j then become combinations of c's, but forming them takes plaintext-plaintext arithmetic, which is not allowed. Instead I keep the ciphertext-side structure and keep the constants inside multiplies.

Plan, with level counts:
- z = x·alpha (1 level, pt mult).
- u = z·z (level 2).
- T2 = 2u − one. Adding `one` as a plaintext costs only an encoding. This is at level 2.
- T4 = 2·T2² − one (level 3).
- T6 = 2·T2·T4 − T2 (level 4).
- Then multiply by the c's.

Cheaper: T6 = 2·T2·T4 − T2, T4 = 2T2²−1. Define w = T2 (level 2). Then T4 uses one ct-ct multiply (level 3) and T6 uses one more (level 4). The result is c1·z + c2·w + c4·T4 + c6·T6 + c0, so the final plaintext multiplies add one more level, giving depth 5 with 4 ct-ct multiplies (u, w², w·T4, plus none for z). That is 3 ct-ct multiplies, down from 5.

Folding alpha: u = (x·alpha)² requires z first, so depth is the same. Alternatively, compute x² (ct-ct, level 1) and then multiply by alpha² — but that needs plaintext-plaintext arithmetic for alpha², so I skip it. I could instead use (x·alpha)·(x·alpha), which is the same as z·z. I'll keep it simple.

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
  Ciphertext w = u + u - one;
  Ciphertext w2 = square(w);
  Ciphertext t4 = w2 + w2 - one;
  Ciphertext wt4 = w * t4;
  Ciphertext t6 = wt4 + wt4 - w;
  Ciphertext result = z * c1;
  result = result + w * c2;
  result = result + t4 * c4;
  result = result + t6 * c6;
  result = result + c0;
  result.set_output("y");
}
```