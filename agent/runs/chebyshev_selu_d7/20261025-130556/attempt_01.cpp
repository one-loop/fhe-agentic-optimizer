void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext T2 = z2 + z2 - one;
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;
  Ciphertext T2s = square(T2);
  Ciphertext T4 = T2s + T2s - one;

  Ciphertext Q = (z * c5) + (T2 * c6) + (T3 * c7);
  Ciphertext inner = (Q + Q) + c4;
  Ciphertext hi = T4 * inner;

  Ciphertext lo = (z * c1) + (T2 * c2) + (T3 * c3);
  Ciphertext m = (z * c7) + (T2 * c6) + (T3 * c5);
  Ciphertext r = hi + lo - m;
  r = r + c0;
  r.set_output("y");
}
