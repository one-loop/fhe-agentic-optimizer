Idea: the polynomial is odd apart from c0, so write it as y = c0 + z·(c1 + c3 T3'(z²) ...). In the monomial basis, p(z) = a1 z + a3 z³ + a5 z⁵ + a7 z⁷ + c0. The coefficients must be computed from the plaintext inputs, but plaintext-plaintext arithmetic is not allowed, so I keep the Chebyshev coefficients and expand symbolically in the ciphertext.

T3 = 4z³ − 3z, T5 = 16z⁵ − 20z³ + 5z, T7 = 64z⁷ − 112z⁵ + 56z³ − 7z.
Folding the integer factors into constants is not possible without plaintext arithmetic, but they can be built from ciphertext additions, which cost little.

Cheaper structure: let u = z² (one ct-ct square). Then
y = c0 + z·(c1 + c3(4u − 3) + c5(16u² − 20u + 5) + c7(64u³ − 112u² + 56u − 7)).
That is a polynomial Q(u) of degree 3 in u. Computing it needs u², then u³ or a Horner step.

The integer multiples use additions: 4u = 4 doublings, which are cheap. I would rather avoid many ct-pt multiplies, so I form the following:
- A = c7·(64u − 112) + c5·16, so Q = c1 − 3c3 + 5c5 − 7c7 + u·(4c3 − 20c5 + 56c7) + u²·(16c5 − 112c7) + u³·64c7.
Each coefficient is a plaintext combination of integer multiples, and those combinations need plaintext arithmetic, which is not allowed. Instead I multiply each ciphertext by its plaintext and use doublings on the ciphertext side.

Plan with Horner-like grouping, using the ciphertext doubling helper dbl(a) = a + a:
- z = x·alpha (1 level)
- u = z² (level 2)
- Define w = u·c7 (ct-pt), then the u-polynomial comes from the terms:
  - P = c7·(64u³ − 112u² + 56u − 7) + c5·(16u² − 20u + 5) + c3·(4u − 3) + c1
  - Group by the multiples: P = (c7·u)·(64u² − 112u + 56) + c5·u·(16u − 20) + 4c3·u + const. This is still quite long, so I just implement it directly with the multiples built from ciphertext additions.

Using T-terms: P = c1 + c3·a3 + c5·a5 + c7·a7, where a3 = 4u − 3z⁰, which needs "one" (a plaintext): a3 = 4u − 3·one, with one added as plaintext, costing ~2.5 ms each.

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
  Plaintext c7("c7");
  auto dbl = [](const Ciphertext &a) { return a + a; };
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext u2 = dbl(u);
  Ciphertext u4 = dbl(u2);
  Ciphertext u16 = dbl(dbl(u4));
  // a3 = 4u - 3
  Ciphertext a3 = u4 - one - one - one;
  // a5 = 16u^2 - 20u + 5 = u*(16u - 20) + 5
  Ciphertext u20 = u16 + u4;
  Ciphertext b5 = u16 - one - one - one - one - one;
  b5 = b5 - one - one - one - one - one - one - one - one - one - one
       - one - one - one - one - one - one - one - one - one - one;
  Ciphertext a5 = u * b5 + one + one + one + one + one;
  // a7 = 64u^3 - 112u^2 + 56u - 7 = u*(u*(64u - 112) + 56) - 7
  Ciphertext u64 = dbl(dbl(u16));
  Ciphertext u112 = u64 + u16 + u16 + u16;
  (void)u20;
  (void)u112;
  (void)a3;
  (void)a5;
  Ciphertext r = z * c1;
  r.set_output("y");
}
```

That draft is incomplete and incorrect, so please discard it. Correct version below: it uses the straightforward Chebyshev recurrence on the odd terms with fewer products.

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
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext t2 = s + s - one;
  Ciphertext t3 = z * t2;
  t3 = t3 + t3 - z;
  Ciphertext t4 = square(t2);
  t4 = t4 + t4 - one;
  Ciphertext t5 = z * t4;
  t5 = t5 + t5 - t3;
  Ciphertext t7 = t3 * t4;
  t7 = t7 + t7 - z;
  Ciphertext result = z * c1 + t3 * c3 + t5 * c5 + t7 * c7 + c0;
  result.set_output("y");
}
```