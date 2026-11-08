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
  Ciphertext w = x * alpha;
  Ciphertext u = w * w;
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
  Ciphertext s2 = s + s;
  Ciphertext A = s2 + s2 - one - one - one;
  Ciphertext B = A * c6;
  Ciphertext C = t2 * c4;
  Ciphertext inner = B + C + C;
  inner = inner + c2;
  Ciphertext D = inner * t2;
  Ciphertext result = D + w * c1;
  result = result - c4;
  result = result + c0;
  result.set_output("y");
}
