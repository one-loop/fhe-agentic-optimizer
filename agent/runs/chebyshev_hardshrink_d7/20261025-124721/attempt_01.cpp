void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext w2 = w + w;
  Ciphertext v = w2 + w2 - one - one;
  Ciphertext v1 = v - one;
  Ciphertext t3 = z * v1;
  Ciphertext t5 = t3 * v - z;
  Ciphertext t7 = t5 * v - t3;
  Ciphertext result = z * c1 + t3 * c3 + t5 * c5 + t7 * c7;
  result.set_output("y");
}
