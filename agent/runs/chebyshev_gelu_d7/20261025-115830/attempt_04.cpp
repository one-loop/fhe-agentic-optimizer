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
  Ciphertext s = square(w);
  Ciphertext t = w * c6;
  Ciphertext t2 = t + t;
  Ciphertext k = t2 + t2;
  k = k + c4;
  k = k + c4;
  Ciphertext f = s * k;
  Ciphertext m = w * c2;
  m = m - t;
  m = m - t;
  m = m - t;
  m = m - c4;
  m = m + c0;
  Ciphertext g = z * c1;
  m = m + g;
  Ciphertext result = f + m;
  result.set_output("y");
}
