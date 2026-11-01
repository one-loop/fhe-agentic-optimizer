void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext v = u + u - one;
  Ciphertext v2 = square(v);
  Ciphertext v3 = v * v2;

  Ciphertext vv = v + v;
  Ciphertext v2x2 = v2 + v2;
  Ciphertext v2x4 = v2x2 + v2x2;
  Ciphertext v3x2 = v3 + v3;
  Ciphertext v3x4 = v3x2 + v3x2;
  Ciphertext v3x8 = v3x4 + v3x4;

  Ciphertext A3 = vv - one;
  Ciphertext A5 = v2x4 - vv - one;
  Ciphertext A7 = v3x8 - v2x4 - vv + one;
  Ciphertext A2 = v2x2 - one;
  Ciphertext C = v3x4 - vv - v;

  Ciphertext B = A3 * c3;
  B = B + A5 * c5;
  B = B + A7 * c7;
  B = B + c1;

  Ciphertext E = v * c2;
  E = E + A2 * c4;
  E = E + C * c6;
  E = E + c0;

  Ciphertext zB = z * B;
  Ciphertext result = zB + E;
  result.set_output("y");
}
