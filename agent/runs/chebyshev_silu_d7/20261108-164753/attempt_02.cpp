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
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext a = t2 * c6;
  Ciphertext m = a + a + c4;
  Ciphertext p = m * t4;
  Ciphertext d = t2 * c2 - a;
  Ciphertext w = z * c1 + c0;
  Ciphertext result = p + (d + w);
  result.set_output("y");
}
