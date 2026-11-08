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
  Ciphertext w = x * alpha;
  Ciphertext u = w * w;
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext a = t4 * c6;
  Ciphertext m = t2 * a;
  Ciphertext r = m + m;
  r = r + t4 * c4;
  r = r + t2 * c2;
  r = r - t2 * c6;
  r = r + w * c1;
  r = r + c0;
  r.set_output("y");
}
