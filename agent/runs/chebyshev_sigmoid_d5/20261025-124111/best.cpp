void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext t2 = u + u - one;
  Ciphertext a = t2 + t2 - one;
  Ciphertext w = a + a;
  Ciphertext m = w * c5 + c3 + c3;
  Ciphertext inner = t2 * m + c1 - c3 - c5;
  Ciphertext result = inner * z + c0;
  result.set_output("y");
}
