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
  Ciphertext c6w = w * c6;
  Ciphertext q = c6w + c6w + c4;
  Ciphertext q2 = q + q;
  Ciphertext p = w2 * q2;
  Ciphertext term2 = w * c2;
  Ciphertext r = term2 - c6w;
  r = r - q;
  Ciphertext result = p + r;
  Ciphertext term1 = z * c1;
  result = result + term1;
  result = result + c0;
  result.set_output("y");
}
