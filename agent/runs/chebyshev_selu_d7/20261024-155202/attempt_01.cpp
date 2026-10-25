void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext z3 = z * t2;
  Ciphertext t3 = z3 + z3 - z;
  Ciphertext q = t2 * t2;
  Ciphertext t4 = q + q - one;
  Ciphertext a = z * c5 + t2 * c6 + t3 * c7;
  Ciphertext g = a + a + c4;
  Ciphertext b = z * c7 + t2 * c6 + t3 * c5;
  Ciphertext low = z * c1 + t2 * c2 + t3 * c3;
  Ciphertext y = t4 * g + low - b + c0;
  y.set_output("y");
}
