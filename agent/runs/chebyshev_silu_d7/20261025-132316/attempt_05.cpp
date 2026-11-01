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
  Ciphertext v = u + u - one;
  Ciphertext s = square(v);
  Ciphertext t4 = s + s - one;
  Ciphertext p = v * c4;
  p = p + t4 * c6;
  Ciphertext inner = p + p + c2 - c6;
  Ciphertext r = v * inner;
  r = r + z * c1;
  r = r - c4;
  r = r + c0;
  r.set_output("y");
}
