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
  Ciphertext w = square(z);
  Ciphertext t2 = w + w - one;
  Ciphertext s = square(t2);
  Ciphertext t4 = s + s - one;
  Ciphertext r = t2 * c6;
  Ciphertext u = (r + r) + c4;
  Ciphertext m = t4 * u;
  Ciphertext low = (z * c1 + c0) + (t2 * c2 - r);
  Ciphertext result = low + m;
  result.set_output("y");
}
