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
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext w = u + u - one;
  Ciphertext w2 = square(w);
  Ciphertext w4 = w2 + w2;
  w4 = w4 + w4;
  Ciphertext sp = w4 - one - one - one;
  Ciphertext a = w * c4;
  Ciphertext inner = sp * c6;
  inner = inner + a + a;
  inner = inner + c2;
  Ciphertext result = w * inner;
  result = result + z * c1;
  result = result + c0;
  result = result - c4;
  result.set_output("y");
}
