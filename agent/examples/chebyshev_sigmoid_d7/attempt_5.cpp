// Chebyshev doubling formulas T_{m+n} = 2 T_m T_n - T_{|m-n|}: logarithmic depth.
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext t1 = x * alpha;
  Ciphertext s2 = t1 * t1;
  Ciphertext t2 = s2 + s2 - one;
  Ciphertext p12 = t1 * t2;
  Ciphertext t3 = p12 + p12 - t1;
  Ciphertext s4 = t2 * t2;
  Ciphertext t4 = s4 + s4 - one;
  Ciphertext p23 = t2 * t3;
  Ciphertext t5 = p23 + p23 - t1;
  Ciphertext p34 = t3 * t4;
  Ciphertext t7 = p34 + p34 - t1;
  Plaintext c0("c0"), c1("c1"), c3("c3"), c5("c5"), c7("c7");
  Ciphertext y = t1 * c1 + t3 * c3 + t5 * c5 + t7 * c7 + c0;
  y.set_output("y");
}
