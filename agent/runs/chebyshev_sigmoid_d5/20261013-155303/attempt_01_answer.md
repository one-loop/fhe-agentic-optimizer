**Idea:** The baseline builds the Chebyshev recurrence, which costs 4 ciphertext multiplies and depth 6. Since T3 and T5 are odd polynomials in z, I expand the sum into monomials z, z³, z⁵ with z = x/4. That needs only three ciphertext multiplies (z², z³ = z²·z, z⁵ = z³·z²), and the depth drops to 4. Each coefficient is a linear combination of the plaintext coefficients:

- z: c1 − 3c3 + 5c5
- z³: 4c3 − 20c5
- z⁵: 16c5

Integer multipliers (3, 4, 5, 16, 20) are built from ciphertext doublings and additions, which are cheap. Multiplying by c3 or c5 is done as a ciphertext-plaintext multiply, and the terms are summed at different levels to avoid scale mismatches.

```cpp
// Chebyshev sigmoid degree 5 on [-4,4], expanded to monomials in z = x/4:
// y = c0 + c1 z + c3 (4z^3 - 3z) + c5 (16z^5 - 20z^3 + 5z)
//   = c0 + z*(c1 - 3c3 + 5c5) + z^3*(4c3 - 20c5) + z^5*(16c5)

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  Ciphertext z = x * alpha;          // z = x/4
  Ciphertext z2 = z * z;             // z^2
  Ciphertext z3 = z2 * z;            // z^3
  Ciphertext z5 = z3 * z2;           // z^5

  // z-terms
  Ciphertext p1 = z * c1;
  Ciphertext p3 = z * c3;
  Ciphertext p5 = z * c5;

  // z^3 and z^5 terms
  Ciphertext q3 = z3 * c3;
  Ciphertext q5 = z3 * c5;
  Ciphertext r5 = z5 * c5;

  // 4*c3*z^3
  Ciphertext a4 = q3 + q3;
  a4 = a4 + a4;

  // 16*c5*z^5
  Ciphertext b16 = r5 + r5;
  b16 = b16 + b16;
  b16 = b16 + b16;
  b16 = b16 + b16;

  // 20*c5*z^3 = 16*q5 + 4*q5
  Ciphertext q4 = q5 + q5;
  q4 = q4 + q4;
  Ciphertext q16 = q4 + q4;
  q16 = q16 + q16;
  Ciphertext t20 = q16 + q4;

  // 3*c3*z
  Ciphertext p3x3 = p3 + p3;
  p3x3 = p3x3 + p3;

  // 5*c5*z = p5 + 4*p5
  Ciphertext p5x4 = p5 + p5;
  p5x4 = p5x4 + p5x4;
  Ciphertext p5x5 = p5 + p5x4;

  Ciphertext result = p1 + a4;
  result = result + b16;
  result = result - t20;
  result = result - p3x3;
  result = result + p5x5;
  result = result + c0;
  result.set_output("y");
}
```