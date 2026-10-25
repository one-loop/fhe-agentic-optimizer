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
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(t2);
  Ciphertext t4 = s + s - one;
  Ciphertext tc = t4 * c6;
  Ciphertext inner = tc + tc - c6 + c2;
  Ciphertext result = z * c1;
  result = result + t4 * c4;
  result = result + t2 * inner;
  result = result + c0;
  result.set_output("y");
}
