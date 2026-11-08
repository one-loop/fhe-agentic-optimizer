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
  Ciphertext a = t2 * c6;
  Ciphertext p = a * t4;
  Ciphertext result = p + p - a;
  result = result + t4 * c4;
  result = result + t2 * c2;
  result = result + z * c1;
  result = result + c0;
  result.set_output("y");
}
