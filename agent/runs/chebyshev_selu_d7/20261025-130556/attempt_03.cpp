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

  Ciphertext zc5 = z * c5;
  Ciphertext zc7 = z * c7;
  Ciphertext U = T2 * c6;
  Ciphertext Q = (zc5 + U) + (T3 * c7);
  Ciphertext inner = (Q + Q) + c4;
  Ciphertext P = T2s * inner;
  Ciphertext hi = (P + P) - inner;

  Ciphertext lo = ((z * c1) + c0) + (T2 * c2);
  Ciphertext m = (zc7 + U) + (T3 * c5);
  Ciphertext r = ((lo - m) + (T3 * c3)) + hi;
  r.set_output("y");
}
