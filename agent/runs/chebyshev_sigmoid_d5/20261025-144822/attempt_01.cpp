void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext u2 = u + u;
  Ciphertext s = u2 + u2;
  Ciphertext s5 = s - one;
  s5 = s5 - one;
  s5 = s5 - one;
  s5 = s5 - one;
  s5 = s5 - one;
  Ciphertext r = s * s5;
  Ciphertext r5 = r + one;
  r5 = r5 + one;
  r5 = r5 + one;
  r5 = r5 + one;
  r5 = r5 + one;
  Ciphertext s3 = s - one;
  s3 = s3 - one;
  s3 = s3 - one;
  Ciphertext g = r5 * c5 + s3 * c3;
  g = g + c1;
  Ciphertext y = z * g;
  y = y + c0;
  y.set_output("y");
}
