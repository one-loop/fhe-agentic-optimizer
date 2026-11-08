void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = c2 * z;
  Ciphertext u = c1 + (w + w);
  Ciphertext p = u * z;
  Ciphertext y = c0 + (p - c2);
  y.set_output("y");
}
