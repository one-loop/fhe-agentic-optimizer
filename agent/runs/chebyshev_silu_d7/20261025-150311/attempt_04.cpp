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
  Ciphertext t2 = u + u - one;
  Ciphertext s2 = square(t2);
  Ciphertext a = t2 * c6;
  Ciphertext w = a + a + c4;
  Ciphertext w2 = w + w;
  Ciphertext prod = s2 * w2;
  Ciphertext low = t2 * c2 - a - a - a;
  low = low + z * c1;
  low = low - c4;
  low = low + c0;
  Ciphertext result = prod + low;
  result.set_output("y");
}
