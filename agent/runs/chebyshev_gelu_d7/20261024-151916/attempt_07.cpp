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
  Ciphertext a = w * c6;
  Ciphertext b = a * (t4 + t4);
  Ciphertext result = b - a;
  result = result + w * c2;
  Ciphertext d = t4 * c4;
  Ciphertext e = z * c1;
  result = result + (d + e);
  result = result + c0;
  result.set_output("y");
}
