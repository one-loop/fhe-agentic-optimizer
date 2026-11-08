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
  Ciphertext m = e3 * c7;
  m = m + c5;
  Ciphertext n = t4 * m;
  Ciphertext a = e3 * c3;
  Ciphertext b = e3 * c5;
  Ciphertext s = n + n + a;
  s = s - b;
  s = s - c7;
  s = s + c1;
  Ciphertext result = z * s + c0;
  result.set_output("y");
}
