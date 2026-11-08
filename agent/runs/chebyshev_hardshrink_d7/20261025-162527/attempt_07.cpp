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
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext p5 = t2 * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext p7 = t2 * t5;
  Ciphertext t7 = p7 + p7 - t3;
  Ciphertext a = t7 * c7;
  Ciphertext b = t5 * c5;
  Ciphertext c = t3 * c3;
  Ciphertext d = z * c1;
  Ciphertext result = a + (b + (c + d));
  result.set_output("y");
}
