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
  Ciphertext a = t2 * c6;
  Ciphertext a2 = a + a;
  Ciphertext a4 = a2 + a2;
  Ciphertext b = a4 + c4;
  b = b + c4;
  Ciphertext w = s * b;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result - a2;
  result = result - a;
  result = result + w;
  result = result + c0;
  result = result - c4;
  result.set_output("y");
}
