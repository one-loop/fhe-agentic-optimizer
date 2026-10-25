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
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext u2 = t2 * t2;
  Ciphertext a6 = t2 * c6;
  Ciphertext m = a6 + a6 + c4;
  Ciphertext pm = u2 * m;
  Ciphertext low = t2 * c2 + z * c1;
  low = low - a6 - m;
  Ciphertext result = pm + pm + low;
  result = result + c0;
  result.set_output("y");
}
