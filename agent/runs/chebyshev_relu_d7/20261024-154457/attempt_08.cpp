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
  Ciphertext t2 = u + u - one;
  Ciphertext u2 = t2 * t2;
  Ciphertext t4 = u2 + u2 - one;
  Ciphertext a6 = t2 * c6;
  Ciphertext m = a6 + a6 + c4;
  Ciphertext p = t4 * m;
  Ciphertext mid = t2 * c2 - a6;
  Ciphertext s = p + mid;
  Ciphertext result = s + z * c1;
  result = result + c0;
  result.set_output("y");
}
