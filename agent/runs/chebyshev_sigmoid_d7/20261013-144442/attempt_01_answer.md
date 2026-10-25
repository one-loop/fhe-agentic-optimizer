Replayed attempt_1.cpp.
```cpp
// Baseline: degree-7 Chebyshev approximation of sigmoid on [-4, 4], evaluated
// exactly like benchmarks/chebyshev_ckks:
//   z = alpha * x, T1 = z, T2 = 2 z^2 - 1, Tk = 2 z T(k-1) - T(k-2),
//   y = c0 + sum_k c_k T_k over the non-negligible coefficients.
// For sigmoid, c2, c4 and c6 are negligible (sigmoid - 1/2 is odd).
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha
  vector<Ciphertext> t(8);
  t[1] = z;
  Ciphertext z_square = z * z;
  t[2] = z_square + z_square - one;
  for (int k = 3; k <= 7; ++k)
  {
    Ciphertext z_t = z * t[k - 1];
    t[k] = z_t + z_t - t[k - 2];
  }
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext y = t[1] * c1;
  y = y + t[3] * c3;
  y = y + t[5] * c5;
  y = y + t[7] * c7;
  y = y + c0;
  y.set_output("y");
}
```