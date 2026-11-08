void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext w = z2 + z2 - one;           // T2
  Ciphertext a = z * w;
  Ciphertext T3 = a + a - z;
  Ciphertext c = w * T3;
  Ciphertext T5 = c + c - z;
  Ciphertext f = w * T5;
  Ciphertext T7 = f + f - T3;

  // even part by Clenshaw in w
  Ciphertext t = w * c6;
  Ciphertext b2 = t + t + c4;
  Ciphertext p = w * b2;
  Ciphertext b1 = p + p + c2 - c6;
  Ciphertext even = w * b1 - b2 + c0;

  Ciphertext E = even + z * c1;
  Ciphertext O = T3 * c3 + T7 * c7;
  Ciphertext R = O + E;
  R = R + T5 * c5;
  R.set_output("y");
}
