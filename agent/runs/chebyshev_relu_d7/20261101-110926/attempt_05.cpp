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
  Ciphertext odd = z * c1;           // level 2
  Ciphertext zz = square(z);         // level 2
  Ciphertext u = zz + zz - one;      // T2(z)
  Ciphertext u2 = square(u);         // level 3

  Ciphertext v = u * c6;             // c6*u, level 3
  Ciphertext cu = u * c2;            // c2*u, level 3
  Ciphertext q = v + v + c4;         // 2 c6 u + c4

  Ciphertext low = (cu - v - q) + c0; // level 3: (c2-3c6)u - c4 + c0
  Ciphertext low2 = low + odd;
  Ciphertext r = u2 * q;             // level 4
  Ciphertext high = r + r;           // 2 u^2 q

  Ciphertext y = low2 + high;
  y.set_output("y");
}
