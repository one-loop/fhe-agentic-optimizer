void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = (-x) * a;
  Ciphertext y = b - t;
  y.set_output("y");
}
