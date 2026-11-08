void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext x2 = x + x;
  Ciphertext g2 = x2 * c2;
  Ciphertext w = g2 * alpha;
  Ciphertext m = w + c1;
  Ciphertext r = m * z;
  r = r - c2;
  r = r + c0;
  r.set_output("y");
}
