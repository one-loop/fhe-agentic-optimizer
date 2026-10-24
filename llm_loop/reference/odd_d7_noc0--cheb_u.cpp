// Reference for degree-7 Chebyshev kernels with only odd terms c1, c3, c5, c7
// (hardshrink). Same derivation as odd_d7--cheb_u.cpp without the constant c0:
//   y = z Q(u), Q = (c1 - c3 - c5 + c7) + 2 (c3 u - c5 u - 2 c7 u) + 4 u^2 (c5 - c7 + 2 c7 u).
// Expected: 5 levels, 4 ct x ct, 4 ct x pt (alpha, c3, c5, c7).
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1"), c3("c3"), c5("c5"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = z * z;
  Ciphertext u = zz + zz - one;
  Ciphertext c3u = u * c3;
  Ciphertext c5u = u * c5;
  Ciphertext c7u = u * c7;
  Ciphertext inner = c7u + c7u + c5 - c7;
  Ciphertext uu = u * u;
  Ciphertext high = uu * inner;
  Ciphertext d1u = c3u - c5u - c7u - c7u;
  Ciphertext high2 = high + high;
  Ciphertext q = (high2 + high2) + (d1u + d1u) + c1 - c3 - c5 + c7;
  Ciphertext y = z * q;
  y.set_output("y");
}
