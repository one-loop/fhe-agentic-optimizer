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

  Ciphertext v = u * c6;             // c6*u, level 3
  Ciphertext cu = u * c2;            // c2*u, level 3
  Ciphertext q = v + v + c4;         // 2 c6 u + c4
  Ciphertext r = u2 * q;             // level 4
  Ciphertext high = r + r;           // 4 c6 u^3 + 2 c4 u^2

  Ciphertext lin = cu - (v + v + v); // (c2 - 3 c6) u
  Ciphertext odd = z * c1;           // level 2

  Ciphertext y = high + lin + odd - c4 + c0;
  y.set_output("y");
}
