The baseline spends one ciphertext multiply on z*z, plus separate scalar multiplies by c1 and c2, and then aligns the two terms. This version uses Horner's form in z: y = (c0 - c2) + z·(2c2·z + c1). That leaves one ciphertext-ciphertext multiply and two scalar multiplies (alpha and c2), so the expensive level-aligned c1 product disappears. The remaining plaintext additions (c1, -c2, c0) are cheap compared with the multiplies.

```cpp
// Horner form of the degree-2 Chebyshev approximation in z = x/4:
//   y = c0 - c2 + z * (2*c2*z + c1)
// One ciphertext-ciphertext multiply, two scalar plaintext multiplies.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");

  Ciphertext z = x * alpha;          // z in [-1, 1], depth 1
  Ciphertext a = z * c2;             // c2 * z, depth 2
  Ciphertext b = a + a;              // 2 * c2 * z (same level, no alignment)
  Ciphertext inner = b + c1;         // 2 c2 z + c1, depth 2
  Ciphertext p = inner * z;          // z * (2 c2 z + c1), depth 3
  Ciphertext q = p - c2;             // subtract c2
  Ciphertext y = q + c0;             // add c0
  y.set_output("y");
}
```