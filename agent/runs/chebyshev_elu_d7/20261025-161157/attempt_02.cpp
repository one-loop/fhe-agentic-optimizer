void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext t2 = zz + zz - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = square(t2);
  Ciphertext p23 = t2 * t3;
  Ciphertext t3s = square(t3);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext p34 = t3 * t4;

  Ciphertext a4 = t2s * c4;
  Ciphertext a5 = p23 * c5;
  Ciphertext a6 = t3s * c6;
  Ciphertext a7 = p34 * c7;
  Ciphertext a3 = zt2 * c3;

  // c4*T4 = 2*a4 - c4 ; c5*T5 = 2*a5 - z*c5 ; c6*T6 = 2*a6 - c6 ; c7*T7 = 2*a7 - z*c7
  Ciphertext zc = z * c1;
  Ciphertext zc3 = z * c3;
  Ciphertext zc5 = z * c5;
  Ciphertext zc7 = z * c7;
  Ciphertext t2c = t2 * c2;

  Ciphertext hi = a5 + a5 - zc5;
  hi = hi + (a6 + a6);
  hi = hi + (a7 + a7 - zc7);
  Ciphertext lo = a3 + a3 - zc3;
  lo = lo + (a4 + a4);
  Ciphertext mid = t2c + zc;
  Ciphertext r = hi + lo;
  r = r + mid;
  r = r - c4;
  r = r - c6;
  r = r + c0;
  r.set_output("y");
}
