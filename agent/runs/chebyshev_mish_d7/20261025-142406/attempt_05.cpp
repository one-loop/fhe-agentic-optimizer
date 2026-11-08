void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext w = u + u - one;
  Ciphertext w2 = w + w;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext e2 = w2 * c6 + c4;
  Ciphertext e1 = w2 * e2 + c2 - c6;
  Ciphertext ev = w * e1 + c0 - e2;
  Ciphertext d2 = w2 * c7 + c5;
  Ciphertext d1 = w2 * d2 + c3 - c7;
  Ciphertext d0 = w2 * d1 + c1 - d2;
  Ciphertext dd = d0 - d1;
  Ciphertext od = z * dd;
  Ciphertext y = od + ev;
  y.set_output("y");
}
