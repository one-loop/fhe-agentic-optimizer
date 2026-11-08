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
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext s2 = square(t2);
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext a = t2 * c6;
  Ciphertext q = a + a + c4;
  Ciphertext r = t4 * q;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + r;
  result = result - a;
  result = result + c0;
  result.set_output("y");
}
