void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext T2 = z2 + z2 - one;
  Ciphertext zt2 = z * T2;
  Ciphertext T3 = zt2 + zt2 - z;
  Ciphertext s2 = square(T2);
  Ciphertext T4 = s2 + s2 - one;

  Ciphertext T1d = z + z;
  Ciphertext T2d = T2 + T2;
  Ciphertext T3d = T3 + T3;

  Ciphertext B = T1d * c5 + T2d * c6 + T3d * c7 + c4;
  Ciphertext A = z * c1 + T2 * c2 + T3 * c3 + c0;
  Ciphertext sub = T3 * c5 + T2 * c6 + z * c7;
  Ciphertext r = (A - sub) + T4 * B;
  r.set_output("y");
}
