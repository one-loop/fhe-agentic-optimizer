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
  Ciphertext z2 = square(z);         // level 2
  Ciphertext u = z2 + z2 - one;      // T2, level 2
  Ciphertext u2 = square(u);         // level 3
  Ciphertext t4 = u2 + u2 - one;     // T4, level 3
  Ciphertext a = u * c6;             // level 3
  Ciphertext b = a + a + c4;         // level 3
  Ciphertext q = t4 * b;             // level 4
  Ciphertext lin = u * c2 - a;       // level 3
  Ciphertext l1 = z * c1;            // level 2
  Ciphertext result = q + (lin + (l1 + c0));
  result.set_output("y");
}
