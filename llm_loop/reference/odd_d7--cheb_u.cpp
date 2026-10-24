// Reference for degree-7 Chebyshev kernels with odd terms c1, c3, c5, c7 plus
// a constant c0 (sigmoid; hardshrink has no c0, see odd_d7_noc0).
// T_(2k+1)(z) = z V_k(u) with u = T2(z), V_0 = 1, V_1 = 2u - 1,
// V_2 = 4u^2 - 2u - 1, V_3 = 8u^3 - 4u^2 - 4u + 1, so y = c0 + z Q(u) with
//   Q = (c1 - c3 - c5 + c7) + 2 (c3 u - c5 u - 2 c7 u) + 4 u^2 (c5 - c7 + 2 c7 u).
// Expected: 5 levels, 4 ct x ct, 4 ct x pt (alpha, c3, c5, c7).
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c3("c3"), c5("c5"), c7("c7");
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
  Ciphertext y = z * q + c0;
  y.set_output("y");
}
