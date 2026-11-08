void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = z + z;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zc7 = zz * c7;
  Ciphertext b6 = zc7 + c6;
  Ciphertext b5 = zz * b6 + c5 - c7;
  Ciphertext b4 = zz * b5 + c4 - b6;
  Ciphertext b3 = zz * b4 + c3 - b5;
  Ciphertext b2 = zz * b3 + c2 - b4;
  Ciphertext u = c1 - b3;
  Ciphertext y = t2 * b2 + z * u + c0;
  y.set_output("y");
}
