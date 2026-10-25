void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext s = z2 + z2 - one;
  Ciphertext z4 = z2 * z2;
  Ciphertext a2 = z4 + z4;
  Ciphertext a4 = a2 + a2;
  Ciphertext a8 = a4 + a4;
  Ciphertext b2 = z2 + z2;
  Ciphertext b4 = b2 + b2;
  Ciphertext b8 = b4 + b4;
  Ciphertext q = a8 - b8 + one;
  Ciphertext sq = s * q;
  Ciphertext r = sq + sq - s;
  Ciphertext v1 = s + s - one;
  Ciphertext v2 = q + q - v1;
  Ciphertext v3 = r + r - v2;
  Ciphertext e = s * c2 + q * c4 + r * c6 + c0;
  Ciphertext o = v1 * c3 + v2 * c5 + v3 * c7 + c1;
  Ciphertext y = e + z * o;
  y.set_output("y");
}
