void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext u = square(x) * alpha;
  Ciphertext v = u - one - one;
  Ciphertext a3 = v - one;
  Ciphertext a5 = a3 * v - one;
  Ciphertext cv = v * c7 + c5;
  Ciphertext low = a3 * c3 - a3 * c7 + c1;
  Ciphertext p = a5 * cv + low;
  Ciphertext result = z * p;
  result.set_output("y");
}
