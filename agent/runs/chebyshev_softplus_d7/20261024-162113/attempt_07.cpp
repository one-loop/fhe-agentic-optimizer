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
  Ciphertext u = square(z);
  Ciphertext t2 = u + u - one;
  Ciphertext t4s = square(t2);
  Ciphertext t4 = t4s + t4s - one;
  Ciphertext a = t2 * c6;
  Ciphertext low = z * c1 + c0;
  low = low + t2 * c2;
  low = low - a;
  Ciphertext b = a + a + c4;
  Ciphertext result = t4 * b + low;
  result.set_output("y");
}
