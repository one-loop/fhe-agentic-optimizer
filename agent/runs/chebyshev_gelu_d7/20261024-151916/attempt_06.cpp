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
  Ciphertext t4 = w2 + w2 - one;
  Ciphertext p6 = t4 * c6;
  Ciphertext bracket = p6 + p6;
  bracket = bracket + c2;
  bracket = bracket - c6;
  Ciphertext result = w * bracket;
  result = result + t4 * c4;
  result = result + z * c1;
  result = result + c0;
  result.set_output("y");
}
