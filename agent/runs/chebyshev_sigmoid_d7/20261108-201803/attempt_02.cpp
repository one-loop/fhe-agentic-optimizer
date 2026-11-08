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
  Ciphertext zz = z * z;
  Ciphertext t2 = zz + zz - one;
  Ciphertext s = t2 + t2 - one;
  Ciphertext t3 = z * s;
  Ciphertext t2s = t2 * t2;
  Ciphertext t4 = t2s + t2s - one;

  Ciphertext u = t2 * c5 + t4 * c7;
  Ciphertext v = u + u + c3;
  Ciphertext high = t3 * v;

  Ciphertext lin = z * c1 - z * c5 - z * c7;
  Ciphertext result = high + lin + c0;
  result.set_output("y");
}
