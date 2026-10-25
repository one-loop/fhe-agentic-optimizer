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

  // c6*T6 + c4*T4 = T4 * (2 c6 T2 + c4) - c6 T2
  Plaintext c6("c6");
  Plaintext c4("c4");
  Ciphertext v = T2 * c6;
  Ciphertext u = v + v + c4;
  Ciphertext m = T4 * u;

  Plaintext c2("c2");
  Ciphertext w = T2 * c2;
  Ciphertext wv = w - v;

  Plaintext c1("c1");
  Ciphertext term1 = z * c1;

  Ciphertext result = wv + term1;
  result = result + m;
  Plaintext c0("c0");
  result = result + c0;
  result.set_output("y");
}
