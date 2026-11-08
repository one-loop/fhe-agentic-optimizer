void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext a = z2 + z2 - one;
  Ciphertext u2 = square(a);
  Ciphertext u3 = a * u2;

  Ciphertext a2 = a + a;
  Ciphertext a4 = a2 + a2;
  Ciphertext u2d = u2 + u2;
  Ciphertext u2q = u2d + u2d;
  Ciphertext u3d = u3 + u3;
  Ciphertext u3q = u3d + u3d;
  Ciphertext u3o = u3q + u3q;

  // E = c0 - c4 + c2 a + c4 (2 u2) + c6 (4 u3) - 3 c6 a
  Ciphertext w6 = a * c6;
  Ciphertext w6x3 = w6 + w6 + w6;
  Ciphertext E = a * c2 + u2d * c4 + u3q * c6 - w6x3;
  E = E + c0;
  E = E - c4;

  // O = c1 - c3 - c5 + c7 + c3 (2a) + c5 (4 u2 - 2a) + c7 (8 u3 - 4 u2 - 4 a)
  Ciphertext O = a2 * c3 + u2q * c5 - a2 * c5 + u3o * c7 - u2q * c7 - a4 * c7;
  O = O + c1;
  O = O - c3;
  O = O - c5;
  O = O + c7;

  Ciphertext y = E + z * O;
  y.set_output("y");
}
