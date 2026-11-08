void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext w = z2 + z2 - one;           // T2

  // odd part: z * (b0 - b1), Clenshaw in w
  Ciphertext t7 = w * c7;
  Ciphertext b2 = t7 + t7 + c5;
  Ciphertext p = w * b2;
  Ciphertext b1 = p + p + c3 - c7;
  Ciphertext q = w * b1;
  Ciphertext inner = q + q + c1 - b2 - b1;
  Ciphertext odd = z * inner;

  // even part: Clenshaw in w
  Ciphertext t6 = w * c6;
  Ciphertext e2 = t6 + t6 + c4;
  Ciphertext pe = w * e2;
  Ciphertext e1 = pe + pe + c2 - c6;
  Ciphertext even = w * e1 - e2 + c0;

  Ciphertext R = odd + even;
  R.set_output("y");
}
