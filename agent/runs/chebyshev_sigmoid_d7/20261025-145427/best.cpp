void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c3("c3"), c5("c5"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext a = u + u - one;
  Ciphertext a2 = a + a;
  Ciphertext b3 = a2 - one;
  Ciphertext p5 = a2 * b3;
  Ciphertext b5 = p5 - one;
  Ciphertext p7 = a2 * b5;
  Ciphertext b7 = (p7 - a2) + one;

  Ciphertext q = b3 * c3;
  q = q + b5 * c5;
  q = q + b7 * c7;
  q = q + c1;
  Ciphertext r = z * q;
  r = r + c0;
  r.set_output("y");
}
