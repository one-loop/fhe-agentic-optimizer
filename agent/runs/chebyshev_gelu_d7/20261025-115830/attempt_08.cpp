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
  Ciphertext g = z * c1;
  g = g + c0;
  Ciphertext u = square(z);
  Ciphertext w = u + u - one;
  Ciphertext s = square(w);
  Ciphertext v = s + s - one;
  Ciphertext b = v + v - one;
  Ciphertext inner = b * c6;
  inner = inner + c2;
  Ciphertext r = v * c4;
  r = r + g;
  Ciphertext f = w * inner;
  Ciphertext result = f + r;
  result.set_output("y");
}
