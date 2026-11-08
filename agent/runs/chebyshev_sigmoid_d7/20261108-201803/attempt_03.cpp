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
  Ciphertext s = t2 + t2 - one;
  Ciphertext t2s = t2 * t2;
  Ciphertext t4 = t2s + t2s - one;

  Ciphertext r = z3 - z5;
  Ciphertext a = s * r;
  Ciphertext q = s * z7 + z5;
  Ciphertext b = t4 * q;

  Ciphertext lin = z1 - z7;
  Ciphertext result = b + b + a + lin + c0;
  result.set_output("y");
}
