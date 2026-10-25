The baseline spends one ciphertext-ciphertext multiply on z², plus three plaintext multiplies. I rewrote the polynomial in Horner form: y = c0 - c2 + a·(2·c2·a + c1), where a = alpha·x. This is algebraically equal to c0 + c1·z + 2c2·z² - c2 with z = alpha·x. It needs only one ciphertext-ciphertext multiply (the final a·q) and two ciphertext-plaintext multiplies. The factor 2 comes from `p + p` rather than a constant. The depth is 3, within the 10-level budget.

```cpp
// Horner form of the degree-2 Chebyshev relu approximation:
// y = c0 - c2 + a * (2*c2*a + c1), with a = alpha * x (= z).
// Equivalent to c0 + c1*z + c2*(2z^2 - 1).

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");

  Ciphertext a = x * alpha;          // z = x/4, depth 1
  Ciphertext p = a * c2;             // c2 * z, depth 2
  Ciphertext p2 = p + p;             // 2 * c2 * z, no level cost
  Ciphertext q = p2 + c1;            // 2*c2*z + c1, depth 2
  Ciphertext prod = a * q;           // z * (2*c2*z + c1), depth 3
  Ciphertext y = prod + c0 - c2;     // c0 - c2 + ...
  y.set_output("y");
}
```