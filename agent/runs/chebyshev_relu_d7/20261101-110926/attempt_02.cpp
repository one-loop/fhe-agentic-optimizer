void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext z = x * alpha;          // level 1
  Ciphertext zz = z * z;             // level 2
  Ciphertext u = zz + zz - one;      // T2(z)
  Ciphertext u2 = u * u;             // level 3
  Ciphertext t4 = u2 + u2 - one;     // T4(z)

  Ciphertext v = u * c6;             // c6*u, level 3
  Ciphertext cu = u * c2;            // c2*u, level 3
  Ciphertext q = v + v + c4;         // 2 c6 u + c4
  Ciphertext high = t4 * q;          // level 4

  Ciphertext lin = cu - v;           // (c2 - c6) u
  Ciphertext odd = z * c1;           // level 2

  Ciphertext y = high + lin + odd + c0;
  y.set_output("y");
}
