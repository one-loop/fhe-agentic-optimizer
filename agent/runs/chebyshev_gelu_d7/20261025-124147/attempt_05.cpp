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
  Ciphertext e = w * c6;
  Ciphertext e2 = e + e;
  Ciphertext f1 = e2 + c4;
  Ciphertext f = f1 + f1;
  Ciphertext g = w * f + c2;
  Ciphertext e3 = e2 + e;
  Ciphertext p = w * g - e3;
  p = p - c4;
  Ciphertext r = p + z * c1;
  r = r + c0;
  r.set_output("y");
}
