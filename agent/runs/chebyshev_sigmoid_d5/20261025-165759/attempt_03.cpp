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
  Ciphertext s2 = s + s;
  Ciphertext s4 = s2 + s2;
  Ciphertext t = s4 - one;
  t = t - one;
  t = t - one;
  Ciphertext m = t * c5;
  Ciphertext r = m + c3;
  r = r + c5;
  Ciphertext P = r * t;
  P = P + c1;
  P = P - c5;
  Ciphertext y = z * P;
  y = y + c0;
  y.set_output("y");
}
