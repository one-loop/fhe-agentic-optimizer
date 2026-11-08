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
  Ciphertext s = square(z);
  Ciphertext u = s + s - one;
  Ciphertext q = square(u);
  Ciphertext a = u * c6;
  Ciphertext a2 = a + a;
  Ciphertext b = a2 + c4;
  Ciphertext m = q * b;
  Ciphertext r = z * c1 + u * c2;
  r = r - (a2 + a);
  r = r + (m + m);
  r = r + c0;
  r = r - c4;
  r.set_output("y");
}
