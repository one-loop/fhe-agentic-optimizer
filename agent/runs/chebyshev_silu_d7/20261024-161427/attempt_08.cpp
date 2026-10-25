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
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext q = t4 + t4 - one;
  Ciphertext t2d = t2 + t2;
  Ciphertext w = q * c6 + c2 + t2d * c4;
  Ciphertext r = t2 * w;
  Ciphertext result = z * c1;
  result = result + r;
  result = result - c4;
  result = result + c0;
  result.set_output("y");
}
