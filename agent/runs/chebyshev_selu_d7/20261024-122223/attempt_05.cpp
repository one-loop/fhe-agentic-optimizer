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
  Ciphertext t2sq = square(t2);
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext zt4 = z * t4;
  Ciphertext t5 = zt4 + zt4 - t3;
  Ciphertext zt5 = z * t5;
  Ciphertext t6 = zt5 + zt5 - t4;
  Ciphertext zt6 = z * t6;
  Ciphertext t7 = zt6 + zt6 - t5;

  Ciphertext r = z * c1;
  r = r + t2 * c2;
  Ciphertext g = t3 * c3 + t4 * c4;
  r = r + g;
  r = r + t5 * c5;
  r = r + t6 * c6;
  r = r + t7 * c7;
  r = r + c0;
  r.set_output("y");
}
