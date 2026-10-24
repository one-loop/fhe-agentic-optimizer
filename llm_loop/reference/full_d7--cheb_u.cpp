// Reference for degree-7 Chebyshev kernels with every coefficient c0..c7
// (elu, selu, mish): the even part as in even_d7--cheb_u.cpp plus the odd
// part as in odd_d7--cheb_u.cpp, sharing u = T2(z) and u^2:
//   y = [c0 - c4 + (c2 u - 3 c6 u) + 2 u^2 (c4 + 2 c6 u)] + z Q(u),
//   Q = (c1 - c3 - c5 + c7) + 2 (c3 u - c5 u - 2 c7 u) + 4 u^2 (c5 - c7 + 2 c7 u).
// Expected: 5 levels, 5 ct x ct, 6 ct x pt (alpha, c2, c3, c5, c6, c7).
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = z * z;
  Ciphertext u = zz + zz - one;
  Ciphertext uu = u * u;

  Ciphertext c6u = u * c6;
  Ciphertext even_inner = c6u + c6u + c4;
  Ciphertext even_high = uu * even_inner;
  Ciphertext even = (even_high + even_high) + (u * c2 - (c6u + c6u + c6u)) + c0 - c4;

  Ciphertext c3u = u * c3;
  Ciphertext c5u = u * c5;
  Ciphertext c7u = u * c7;
  Ciphertext odd_inner = c7u + c7u + c5 - c7;
  Ciphertext odd_high = uu * odd_inner;
  Ciphertext d1u = c3u - c5u - c7u - c7u;
  Ciphertext odd_high2 = odd_high + odd_high;
  Ciphertext q = (odd_high2 + odd_high2) + (d1u + d1u) + c1 - c3 - c5 + c7;

  Ciphertext y = even + z * q;
  y.set_output("y");
}
