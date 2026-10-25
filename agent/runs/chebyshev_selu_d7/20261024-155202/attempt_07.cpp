void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext s = z2 + z2 - one;
  Ciphertext s2 = s * s;
  Ciphertext q = s2 + s2 - one;
  Ciphertext sq = s * q;
  Ciphertext r = sq + sq - s;
  Ciphertext v1 = s + s - one;
  Ciphertext v2 = q + q - v1;
  Ciphertext v3 = r + r - v2;
  Ciphertext g = s * c6;
  Ciphertext h = g + g + c4;
  Ciphertext e = s * c2 - g + q * h + c0;
  Ciphertext o = v1 * c3 + v2 * c5 + v3 * c7 + c1;
  Ciphertext y = e + z * o;
  y.set_output("y");
}
