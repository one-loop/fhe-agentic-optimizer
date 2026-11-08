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
  Ciphertext s = square(z);
  Ciphertext t2 = s + s - one;
  Ciphertext t3 = z * t2;
  t3 = t3 + t3 - z;
  Ciphertext t4 = square(t2);
  t4 = t4 + t4 - one;
  Ciphertext t5 = z * t4;
  t5 = t5 + t5 - t3;
  Ciphertext t7 = t3 * t4;
  t7 = t7 + t7 - z;
  Ciphertext result = z * c1 + t3 * c3 + t5 * c5 + t7 * c7 + c0;
  result.set_output("y");
}
