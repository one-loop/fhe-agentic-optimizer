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
  Ciphertext w = u + u - one;
  Ciphertext s = w * w;
  Ciphertext s2 = s + s;
  Ciphertext s4 = s2 + s2;
  Ciphertext m = w * c6;
  Ciphertext m3 = m + m + m;
  Ciphertext cubic = s4 * m - m3;
  Ciphertext t4 = s2 - one;
  Ciphertext r = cubic + t4 * c4;
  r = r + w * c2;
  r = r + z * c1;
  r = r + c0;
  r.set_output("y");
}
