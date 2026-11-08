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
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext p5 = t2 * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext d2 = t2 + t2;
  Ciphertext w = d2 * c7 + c5;
  Ciphertext low = z * c1 + t3 * c3;
  Ciphertext result = t5 * w + low - t3 * c7;
  result.set_output("y");
}
