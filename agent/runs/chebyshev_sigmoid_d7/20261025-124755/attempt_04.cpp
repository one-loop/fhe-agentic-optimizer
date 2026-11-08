void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext t = u + u - one;
  Ciphertext e3 = t + t - one;
  Ciphertext t4 = square(t);
  t4 = t4 + t4 - one;
  Ciphertext q = t4 * c7;
  Ciphertext w = q + q + c3;
  w = w - c5;
  Ciphertext r = t4 * c5;
  Ciphertext s = e3 * w + r + r;
  s = s - c7;
  s = s + c1;
  Ciphertext result = z * s + c0;
  result.set_output("y");
}
