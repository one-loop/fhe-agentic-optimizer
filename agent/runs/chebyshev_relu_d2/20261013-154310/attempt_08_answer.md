I'm resubmitting the attempt-3 kernel. It measured 81.18 ms, the fastest of the verified runs. Attempt 7 measured 85.42 ms with an identical operation count, so the timings vary by several milliseconds between runs of the same circuit, and I can't tell the variants apart with the measurements available.

The circuit uses one ciphertext-ciphertext multiply, two ciphertext-plaintext multiplies, and one mod switch from the final constant addition. I don't have a rewrite that removes any of these without a plaintext-plaintext operation, which CKKS forbids, so I'm not using my last attempt on a variant.

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