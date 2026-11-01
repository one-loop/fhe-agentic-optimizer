void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;

  Ciphertext z2 = z * z;
  Ciphertext z3 = z2 * z;
  Ciphertext z4 = z2 * z2;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  // low part: c0 + c1 z + c2 z^2 + c3 z^3
  Ciphertext lo = z * c1;
  Ciphertext t2 = z2 * c2;
  Ciphertext t3 = z3 * c3;
  lo = lo + t2;
  lo = lo + t3;
  lo = lo + c0;

  // high part: c4 + c5 z + c6 z^2 + c7 z^3
  Ciphertext hi = z * c5;
  Ciphertext u2 = z2 * c6;
  Ciphertext u3 = z3 * c7;
  hi = hi + u2;
  hi = hi + u3;
  hi = hi + c4;

  Ciphertext prod = z4 * hi;
  Ciphertext result = lo + prod;
  result.set_output("y");
}
