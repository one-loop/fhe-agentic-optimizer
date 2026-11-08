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
  Ciphertext p = t2 * t4;
  Ciphertext t6 = p + p - t2;
  Ciphertext result = w * c1;
  result = result + t2 * c2;
  result = result + t4 * c4;
  result = result + t6 * c6;
  result = result + c0;
  result.set_output("y");
}
