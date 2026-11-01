void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext s = z * z;
  Ciphertext w = s + s - one;
  Ciphertext zw = z * w;
  Ciphertext t3 = zw + zw - z;
  Ciphertext ww = w * w;
  Ciphertext t4 = ww + ww - one;
  Ciphertext p5 = w * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext p7 = t4 * t3;
  Ciphertext t7 = p7 + p7 - z;
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Plaintext c0("c0");
  Ciphertext result = z * c1;
  result = result + t3 * c3;
  result = result + t5 * c5;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
