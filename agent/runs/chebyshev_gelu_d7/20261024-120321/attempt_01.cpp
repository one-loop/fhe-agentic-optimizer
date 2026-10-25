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
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext c6t2 = t2 * c6;
  Ciphertext p = c6t2 * t4;
  Ciphertext term6 = p + p - c6t2;
  Ciphertext term1 = z * c1;
  Ciphertext term2 = t2 * c2;
  Ciphertext term4 = t4 * c4;
  Ciphertext result = term1 + term2;
  result = result + term4;
  result = result + term6;
  result = result + c0;
  result.set_output("y");
}
