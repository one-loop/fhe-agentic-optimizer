void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext u2 = t2 * t2;
  Ciphertext t4 = u2 + u2 - one;
  Ciphertext t2t4 = t2 * t4;
  Ciphertext t6 = t2t4 + t2t4 - t2;
  Ciphertext result = z * c1 + t2 * c2 + t4 * c4 + t6 * c6;
  result = result + c0;
  result.set_output("y");
}
