void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = t2 * t2;
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext t3t2 = t3 * t2;
  Ciphertext t5 = t3t2 + t3t2 - z;
  Ciphertext t3s = t3 * t3;
  Ciphertext t6 = t3s + t3s - one;
  Ciphertext t4t3 = t4 * t3;
  Ciphertext t7 = t4t3 + t4t3 - z;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext r = z * c1;
  r = r + t2 * c2;
  r = r + t3 * c3;
  r = r + t4 * c4;
  r = r + t5 * c5;
  r = r + t6 * c6;
  r = r + t7 * c7;
  r = r + c0;
  r.set_output("y");
}
