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
  Ciphertext s = square(t2);
  Ciphertext t4 = s + s - one;
  Ciphertext m = t2 * t4;
  Ciphertext t6 = m + m - t2;
  Ciphertext r = z * c1;
  r = r + t2 * c2;
  r = r + t4 * c4;
  r = r + t6 * c6;
  r = r + c0;
  r.set_output("y");
}
