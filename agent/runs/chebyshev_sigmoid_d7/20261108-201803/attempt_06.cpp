void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext z1 = z * c1;
  Ciphertext z3 = z * c3;
  Ciphertext z5 = z * c5;
  Ciphertext z7 = z * c7;

  Ciphertext zz = z * z;
  Ciphertext t2 = zz + zz - one;
  Ciphertext t2s = t2 * t2;

  Ciphertext p = t2 * z7;
  Ciphertext q = p + p + (z5 - z7);
  Ciphertext g = t2s * q;
  Ciphertext g2 = g + g;
  Ciphertext g4 = g2 + g2;

  Ciphertext r = z3 - z5;
  Ciphertext a = t2 * r;
  Ciphertext p2 = p + p;
  Ciphertext lin = z1 + z7 - z3 - z5;
  Ciphertext mid = (a + a - p2 - p2) + lin;

  Ciphertext result = g4 + mid + c0;
  result.set_output("y");
}
