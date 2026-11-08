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
  Ciphertext s2 = s + s;
  Ciphertext s4 = s2 + s2;
  Ciphertext g = s4 * c6;
  Ciphertext h = g + c4;
  Ciphertext inner = h + c4;
  Ciphertext m = s * inner;
  m = m + c2;
  m = m - c6;
  m = m - c6;
  m = m - c6;
  Ciphertext r = s * m;
  r = r + z * c1;
  r = r + c0;
  r = r - c4;
  r.set_output("y");
}
