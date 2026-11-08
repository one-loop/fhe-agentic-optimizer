void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext n = -t;
  Ciphertext y = b - n;
  y.set_output("y");
}
