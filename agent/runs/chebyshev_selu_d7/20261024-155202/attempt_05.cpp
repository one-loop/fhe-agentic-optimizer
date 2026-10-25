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
  Ciphertext rc7 = r * c7;
  Ciphertext o = v1 * c3 + v2 * c5 - v2 * c7 + c1 + rc7 + rc7;
  Ciphertext e = s * c2 + q * c4 + r * c6 + c0;
  Ciphertext y = e + z * o;
  y.set_output("y");
}
