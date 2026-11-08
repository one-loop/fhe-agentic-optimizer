Idea: cut one more ct-ct multiply by rewriting the even part as a quadratic in t2 (itself a function of u).

Even part: c0 + c2·T2 + c4·T4 + c6·T6, with T4 = 2t2² − 1 and T6 = 4t2³ − 3t2 (in terms of t2 = T2... in fact T6 = T3(T2) = 4t2³ − 3t2). So the even part is a cubic in t2:
E = c0 − c4 + (c2 − 3c6)·t2 + 2c4·t2² + 4c6·t2³
 = c0 − c4 + t2·(c2 − 3c6) + t2²·(2c4 + 4c6·t2).

Cost: t2² (ct mul), 4c6·t2 (pt mul), and then multiplying by t2² (ct mul). That is the same number of ct mul as before, so there is no saving there. To remove one, I merge the first step: u = z·z, t2 = 2u − 1. Using x directly, z·z = x·x·alpha², and alpha² cannot be precomputed. Instead take zz = (x·alpha)·x as a ct mul, then multiply by alpha again, which is no gain.

Alternative saving: use the factorisation in terms of t2 with fewer pt mults:
- a = c6·t2 (pt mul, level 3)
- b = (a + a) + (a + a) = 4a, then b + c4 + c4 = 4c6·t2 + 2c4 (adds only), which is a ciphertext at level 3
- s = t2² (ct mul, level 3)
- w = s·b (ct mul, level 4)
- the remaining linear term, t2·(c2 − 3c6) = c2·t2 − 3a = c2·t2 − a − a − a
- constant: c0 − c4, written as c0 minus a plaintext-add of c4 (plaintext ops with a ciphertext operand are fine)

This has 3 ct mul and pt mul on z·c1, t2·c2, t2·c6, which is 3 (down from 4). Depth is 4.

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
  Ciphertext a = t2 * c6;
  Ciphertext a2 = a + a;
  Ciphertext a4 = a2 + a2;
  Ciphertext b = a4 + c4;
  b = b + c4;
  Ciphertext w = s * b;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result - a2;
  result = result - a;
  result = result + w;
  result = result + c0;
  result = result - c4;
  result.set_output("y");
}
```