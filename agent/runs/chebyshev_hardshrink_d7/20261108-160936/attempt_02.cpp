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
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext a5 = t2 * c5;
  Ciphertext p5 = a5 * t3;
  Ciphertext s5 = p5 + p5 - z * c5;
  Ciphertext a7 = t3 * c7;
  Ciphertext p7 = a7 * t4;
  Ciphertext s7 = p7 + p7 - z * c7;
  Ciphertext result = z * c1;
  result = result + t3 * c3;
  result = result + s5;
  result = result + s7;
  result.set_output("y");
}
