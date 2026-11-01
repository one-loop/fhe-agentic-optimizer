void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext a5 = t2 * t3;
  Ciphertext t5 = a5 + a5 - z;
  Ciphertext a6 = t2 * t4;
  Ciphertext t6 = a6 + a6 - t2;
  Ciphertext a7 = t3 * t4;
  Ciphertext t7 = a7 + a7 - z;

  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext hi = t5 * c5;
  hi = hi + t6 * c6;
  Ciphertext mid = t4 * c4;
  Ciphertext lo = z * c1;
  lo = lo + t2 * c2;
  lo = lo + t3 * c3;
  Ciphertext result = hi + mid;
  result = result + lo;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
