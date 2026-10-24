c1 is 2, so c1 z = z + z saves a multiplication.
```cpp
// Reference for degree-7 Chebyshev kernels whose odd coefficients c3, c5, c7
// are negligible (gelu, silu, softplus, relu): only c0, c1, c2, c4, c6 remain.
// With u = T2(z) = 2 z^2 - 1: T4 = 2 u^2 - 1 and T6 = 4 u^3 - 3 u, so
//   y = c0 - c4 + c1 z + (c2 u - 3 c6 u) + 2 u^2 (c4 + 2 c6 u).
// Expected: 4 levels, 3 ct x ct, 4 ct x pt (alpha, c1, c2, c6).
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c4("c4"), c6("c6");
  Ciphertext z = x * alpha;
  Ciphertext zz = z * z;
  Ciphertext u = zz + zz - one;
  Ciphertext c6u = u * c6;
  Ciphertext inner = c6u + c6u + c4;
  Ciphertext uu = u * u;
  Ciphertext high = uu * inner;
  Ciphertext mid = u * c2 - (c6u + c6u + c6u);
  Ciphertext low = z + z;
  Ciphertext y = (high + high) + mid + low + c0 - c4;
  y.set_output("y");
}
```
