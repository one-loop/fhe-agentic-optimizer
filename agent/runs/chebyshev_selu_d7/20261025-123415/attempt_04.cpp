void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext d = t2 + t2 - one;
  Ciphertext t3 = z * d;
  Ciphertext sq = z2 * z2;
  Ciphertext h = sq + sq;
  h = h + h;
  h = h + h;
  Ciphertext g = z2 + z2;
  g = g + g;
  g = g + g;
  Ciphertext t4 = h - g + one;
  Ciphertext u = t2 * c6;
  Ciphertext q = z * c5 + u + t3 * c7;
  Ciphertext q2 = q + q + c4;
  Ciphertext r = z * c7 + u + t3 * c5;
  Ciphertext m = t4 * q2;
  Ciphertext low = z * c1 + t2 * c2 + t3 * c3;
  Ciphertext y = low - r + m;
  y = y + c0;
  y.set_output("y");
}
