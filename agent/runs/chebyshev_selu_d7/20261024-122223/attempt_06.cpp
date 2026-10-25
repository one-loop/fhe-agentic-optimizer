void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext zt3 = z * t3;
  Ciphertext t4 = zt3 + zt3 - t2;
  Ciphertext p25 = t2 * t3;
  Ciphertext t5 = p25 + p25 - z;
  Ciphertext zt5 = z * t5;
  Ciphertext t6 = zt5 + zt5 - t4;
  Ciphertext zt6 = z * t6;
  Ciphertext t7 = zt6 + zt6 - t5;

  Ciphertext r = z * c1;
  r = r + t2 * c2;
  r = r + t3 * c3;
  Ciphertext g = t4 * c4 + t5 * c5;
  r = r + g;
  r = r + t6 * c6;
  r = r + t7 * c7;
  r = r + c0;
  r.set_output("y");
}
