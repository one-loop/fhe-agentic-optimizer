void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;
  Plaintext one("one");

  // T2 = 2 z^2 - 1
  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;

  // T4 = 2 T2^2 - 1
  Ciphertext q = T2 * T2;
  Ciphertext T4 = q + q - one;

  // T6 = 2 T4 T2 - T2
  Ciphertext p = T4 * T2;
  Ciphertext T6 = p + p - T2;

  Plaintext c1("c1");
  Ciphertext term1 = z * c1;
  Plaintext c2("c2");
  Ciphertext term2 = T2 * c2;
  Plaintext c4("c4");
  Ciphertext term4 = T4 * c4;
  Plaintext c6("c6");
  Ciphertext term6 = T6 * c6;

  Ciphertext result = term1 + term2;
  result = result + term4;
  result = result + term6;
  Plaintext c0("c0");
  result = result + c0;
  result.set_output("y");
}
