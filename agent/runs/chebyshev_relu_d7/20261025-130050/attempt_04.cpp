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
  Ciphertext t = u + u - one;
  Ciphertext s = square(t);
  Ciphertext tc6 = t * c6;
  Ciphertext tc6_2 = tc6 + tc6;
  Ciphertext g = tc6_2 + tc6_2 + c4 + c4;
  Ciphertext low = t * c2 - (tc6_2 + tc6) + z * c1 + c0 - c4;
  Ciphertext result = s * g + low;
  result.set_output("y");
}
