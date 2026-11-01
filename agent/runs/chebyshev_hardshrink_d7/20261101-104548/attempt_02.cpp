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
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;

  Ciphertext a = t2 * c5;
  Ciphertext b = t4 * c7;
  Ciphertext inner = a + a + b + b + c3;

  Ciphertext m = (t2 + t2 - one) * inner + c1 - c5 - c7;
  Ciphertext result = z * m;
  result.set_output("y");
}
