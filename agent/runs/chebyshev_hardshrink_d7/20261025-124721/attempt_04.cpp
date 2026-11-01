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
  Ciphertext a3 = v - one;
  Ciphertext a5 = a3 * v - one;
  Ciphertext cv = v * c7;
  Ciphertext c7a7 = a5 * cv - a3 * c7;
  Ciphertext p = a3 * c3 + a5 * c5 + c7a7 + c1;
  Ciphertext result = z * p;
  result.set_output("y");
}
