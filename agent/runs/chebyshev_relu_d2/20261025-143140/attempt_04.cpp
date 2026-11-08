void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = z * c2;
  Ciphertext r = z * (w + w);
  Ciphertext t = z * c1;
  Ciphertext y = ((r + t) + c0) - c2;
  y.set_output("y");
}
