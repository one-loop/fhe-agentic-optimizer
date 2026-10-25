void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext w = zz + zz - one;
  Ciphertext w2 = w + w;

  // even part
  Ciphertext eb2 = (w2 * c6) + c4;
  Ciphertext eb1 = ((w2 * eb2) + c2) - c6;
  Ciphertext e = ((w * eb1) + c0) - eb2;

  // odd part
  Ciphertext ob2 = (w2 * c7) + c5;
  Ciphertext ob1 = ((w2 * ob2) + c3) - c7;
  Ciphertext ob0 = ((w2 * ob1) + c1) - ob2;
  Ciphertext odd = z * (ob0 - ob1);

  Ciphertext y = e + odd;
  y.set_output("y");
}
