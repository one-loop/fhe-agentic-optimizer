void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2sq = square(t2);
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext p25 = t2 * t3;
  Ciphertext t3sq = square(t3);
  Ciphertext p34 = t3 * t4;

  Ciphertext G3 = z2 * c2;
  Ciphertext G4 = zt2 * c3 + t2sq * c4;
  Ciphertext G5 = p25 * c5 + t3sq * c6 + p34 * c7;

  Ciphertext s = G3 + G4;
  s = s + G5;
  Ciphertext d = s + s;

  Ciphertext Zg = z * c1;
  Zg = Zg - z * c3;
  Zg = Zg - z * c5;
  Zg = Zg - z * c7;

  Ciphertext r = d + Zg;
  r = r + c0;
  r = r - c2;
  r = r - c4;
  r = r - c6;
  r.set_output("y");
}
