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
  Ciphertext u = t2 + t2;
  Ciphertext uu = u * u;

  Ciphertext p2 = u * z7;
  Ciphertext q = p2 + (z5 - z7);
  Ciphertext g4 = uu * q;

  Ciphertext r = z3 - z5;
  Ciphertext a2 = u * r;
  Ciphertext lin = z1 + z7 - z3 - z5;
  Ciphertext mid = (a2 - p2 - p2) + lin;

  Ciphertext result = g4 + mid + c0;
  result.set_output("y");
}
