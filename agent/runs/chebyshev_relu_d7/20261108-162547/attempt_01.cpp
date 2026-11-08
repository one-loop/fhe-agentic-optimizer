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
  Ciphertext w = u + u - one;
  Ciphertext w2 = w * w;
  Ciphertext t4 = w2 + w2 - one;
  Ciphertext s = t4 + t4 - one;
  Ciphertext c6w = w * c6;
  Ciphertext term6 = c6w * s;
  Ciphertext term4 = t4 * c4;
  Ciphertext term2 = w * c2;
  Ciphertext term1 = z * c1;
  Ciphertext result = term6 + term4;
  result = result + term2;
  result = result + term1;
  result = result + c0;
  result.set_output("y");
}
