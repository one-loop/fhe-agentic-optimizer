void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext zz = z * z;
  Ciphertext T2 = zz + zz - one;
  Ciphertext W = T2 + T2 - one;
  Ciphertext T3 = z * W;
  Ciphertext T2s = T2 * T2;
  Ciphertext T4 = T2s + T2s - one;

  Ciphertext c6T2 = T2 * c6;
  Ciphertext q = z * c5 + c6T2 + T3 * c7;
  Ciphertext q2 = (q + q) + c4;
  Ciphertext high = T4 * q2;

  Ciphertext low = (z * c1 - z * c7) + (T2 * c2 - c6T2) + (T3 * c3 - T3 * c5);
  Ciphertext result = high + low + c0;
  result.set_output("y");
}
