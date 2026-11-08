void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext zw = z * w;
  Ciphertext zw2 = zw + zw;
  Ciphertext t3 = zw2 + zw2 - z - z - z;
  Ciphertext s2 = t2 * t2;
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext p5 = t3 * t2;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext p7 = t4 * t3;
  Ciphertext t7 = p7 + p7 - z;
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Plaintext c0("c0");
  Ciphertext result = z * c1 + t3 * c3 + t5 * c5 + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
