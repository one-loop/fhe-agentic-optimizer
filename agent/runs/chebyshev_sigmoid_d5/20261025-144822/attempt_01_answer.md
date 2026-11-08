Idea: write the odd polynomial in z as y = c0 + z·(c1 + c3·(4z²−3) + c5·(16z⁴−20z²+5)), which is a polynomial in u = z².

T3 = 4z³ − 3z, T5 = 16z⁵ − 20z³ + 5z.
Sum = c1 z + c3 T3 + c5 T5 = z·(a + b·u + d·u²), where u = z² and
- a = c1 − 3c3 + 5c5
- b = 4c3 − 20c5
- d = 16c5

Computing a, b, d from the plaintext coefficients would be plaintext-plaintext arithmetic, which is not allowed, and constants are not available either. So the coefficient combinations have to be applied through ciphertext-side operations.

Cheaper structure: let u = z², with z = x·alpha.
- Then p = z·(c1 + c3(4u−3) + c5(16u²−20u+5)). The scalar-coefficient multiplies are ct×pt at about 5 ms each, versus 30 ms for ct×ct.
- Alternatively, fold alpha in: x² is a ciphertext square, and u = x²·alpha², which needs alpha² and is plaintext-plaintext. Instead take w = x·alpha (one pt mul), then u = square(w) (one ct mul).
- Evaluate y = c0 + w·(A + B·u + D·u²) with A, B, D as above. Since they cannot be precomputed, build them as ciphertext combinations. Let q = c5·u (ct×pt). Then 16u²−20u+5 = u·(16u−20)+5, which needs constants that cannot be formed.

Fall back to a form that uses only the given plaintexts:
- t3 = z·(4u−1)... T3 = z(4u−3) = z·(u+u+u+u−one−one−one). The constant `one` times a ciphertext would add a level, but adding a plaintext (`+ one`) is allowed (plaintext-vector add).
- Define v = 4u − 3 as ((u+u)+(u+u)) − one − one − one, which needs only plaintext adds and no multiplication.
- T3 = z·v, T5 = 2u·T3 − T1 (since T5 = 2z·T4 − T3, which is more complicated); use T5 = z·(16u²−20u+5). With s = 4u: 16u² = s², so 16u²−20u+5 = s² − 5s + 5, and s·(s−5) + 5 needs one ct×ct.
- Total ct×ct: z² (1), z·v (1), s·(s−5) (1), and z·(...) (1). The poly in u needs c3·v + c5·(s²−5s+5) + c1, then one multiplication by z.
- Plan: z (pt mul), u = z² (mul), s = 4u (adds), r = s·(s−5one) (mul), g = c5·(r+5one) + c3·(s−3one) + c1 (pt muls at the same level, plus pt adds), y = z·g + c0 (mul).

This uses 3 ct×ct and 3 ct×pt, versus 4 and 4 in the baseline, and has lower depth.

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
  Ciphertext u = square(z);
  Ciphertext u2 = u + u;
  Ciphertext s = u2 + u2;
  Ciphertext s5 = s - one;
  s5 = s5 - one;
  s5 = s5 - one;
  s5 = s5 - one;
  s5 = s5 - one;
  Ciphertext r = s * s5;
  Ciphertext r5 = r + one;
  r5 = r5 + one;
  r5 = r5 + one;
  r5 = r5 + one;
  r5 = r5 + one;
  Ciphertext s3 = s - one;
  s3 = s3 - one;
  s3 = s3 - one;
  Ciphertext g = r5 * c5 + s3 * c3;
  g = g + c1;
  Ciphertext y = z * g;
  y = y + c0;
  y.set_output("y");
}
```