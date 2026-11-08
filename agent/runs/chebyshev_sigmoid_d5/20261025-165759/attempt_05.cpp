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
  Ciphertext s = square(z);
  Ciphertext u = s + s;
  u = u - one;
  Ciphertext u2 = u + u;
  Ciphertext t = u2 - one;
  Ciphertext m = u * c5;
  Ciphertext m2 = m + m;
  Ciphertext r = m2 + c3;
  Ciphertext w = z * t;
  Ciphertext y = w * r;
  Ciphertext a = z * c1;
  Ciphertext b = z * c5;
  y = y + a;
  y = y - b;
  y = y + c0;
  y.set_output("y");
}
