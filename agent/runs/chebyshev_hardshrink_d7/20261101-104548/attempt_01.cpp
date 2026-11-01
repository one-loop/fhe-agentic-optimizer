void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext t3 = z * (t2 + t2 - one);
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;

  Ciphertext a = t2 * c5;
  Ciphertext b = t4 * c7;
  Ciphertext inner = a + a + b + b + c3;

  Ciphertext main_part = t3 * inner;
  Ciphertext lin = z * c1 - z * c5 - z * c7;

  Ciphertext result = main_part + lin;
  result.set_output("y");
}
