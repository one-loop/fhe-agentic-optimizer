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
  Ciphertext s = square(z);
  Ciphertext t2 = s + s - one;
  Ciphertext q = square(t2);
  Ciphertext t4 = q + q - one;
  Ciphertext a = t2 * c6;
  Ciphertext a2 = a + a;
  Ciphertext p = a2 * t4;
  Ciphertext u = t2 * c2;
  Ciphertext v = t4 * c4;
  Ciphertext w = z * c1;
  Ciphertext result = p - a + u + v + w + c0;
  result.set_output("y");
}
