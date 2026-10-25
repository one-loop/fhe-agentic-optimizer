Attempt 3 measured 81.18 ms, a 0.04 ms gain over attempt 1, which is within noise. The kernel has converged. I considered alternatives and none reduce the cost:

- Any degree-2 term in x needs one ciphertext-ciphertext multiply, and the rotation-free circuit cannot avoid it.
- The coefficient for x² needs alpha twice and c2 once, and the linear term needs alpha and c1. Folding alpha into a plaintext coefficient would be plaintext-plaintext arithmetic, which is not allowed.
- The remaining mod switch comes from the final constant addition. Removing it would need a constant folded into the product, which again requires a plaintext-plaintext operation.

I'm resubmitting the same kernel as the final answer, since further attempts are unlikely to beat it meaningfully.

```cpp
// Horner form of the degree-2 Chebyshev relu approximation:
// y = c0 - c2 + a * (2*c2*a + c1), with a = alpha * x (= z).
// Equivalent to c0 + c1*z + c2*(2z^2 - 1).
// One ct-ct multiply (a*q) and two ct-pt multiplies (a = x*alpha, p = a*c2).

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