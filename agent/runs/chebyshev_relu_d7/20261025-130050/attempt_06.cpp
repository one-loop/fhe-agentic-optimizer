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
  Ciphertext t = u + u - one;
  Ciphertext s = square(t);
  Ciphertext t4 = s + s - one;
  Ciphertext h = t4 + t4 - one;
  Ciphertext inner = h * c6 + c2;
  Ciphertext result = t * inner + t4 * c4 + z * c1 + c0;
  result.set_output("y");
}
