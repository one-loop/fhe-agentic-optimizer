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
  Ciphertext lowz = z * c1 + c0;
  Ciphertext u = square(z);
  Ciphertext t2 = u + u - one;
  Ciphertext s2 = square(t2);
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext a = t2 * c6;
  Ciphertext w = a + a + c4;
  Ciphertext prod = t4 * w;
  Ciphertext mid = t2 * c2 - a;
  Ciphertext result = prod + (mid + lowz);
  result.set_output("y");
}
