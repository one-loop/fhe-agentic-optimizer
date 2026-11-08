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
  Ciphertext s = u + u - one;
  Ciphertext s2 = square(s);
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext st4 = s * t4;
  Ciphertext t6 = st4 + st4 - s;
  Ciphertext r = z * c1;
  r = r + s * c2;
  r = r + t4 * c4;
  r = r + t6 * c6;
  r = r + c0;
  r.set_output("y");
}
