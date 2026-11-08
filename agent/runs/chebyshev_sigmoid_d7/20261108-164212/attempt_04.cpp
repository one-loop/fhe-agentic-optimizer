void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Plaintext c0("c0");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext u = t2 + t2 - one;
  Ciphertext t3 = z * u;
  Ciphertext s2 = t2 * t2;
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext a = t2 * c5;
  Ciphertext b = t4 * c7;
  Ciphertext inner = a + b;
  inner = inner + inner;
  inner = inner + c3;
  Ciphertext big = t3 * inner;
  Ciphertext zt = z * c1 - z * c5 - z * c7;
  Ciphertext result = big + zt;
  result = result + c0;
  result.set_output("y");
}
