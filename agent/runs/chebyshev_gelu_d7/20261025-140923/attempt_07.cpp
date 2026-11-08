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
  Ciphertext a = u * c6;
  Ciphertext a2 = a + a;
  Ciphertext b = a2 + c4;
  Ciphertext ub = u * b;
  Ciphertext w = (ub + ub) + c2;
  Ciphertext side = z * c1;
  side = side - (a2 + a);
  side = side + c0;
  side = side - c4;
  Ciphertext r = u * w;
  r = r + side;
  r.set_output("y");
}
