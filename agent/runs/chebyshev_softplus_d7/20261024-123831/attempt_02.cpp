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
  Ciphertext cw6 = w * c6;
  Ciphertext s = t4 + t4 - one;
  Ciphertext t6term = cw6 * s;
  Ciphertext result = z * c1;
  result = result + w * c2;
  result = result + t4 * c4;
  result = result + t6term;
  result = result + c0;
  result.set_output("y");
}
