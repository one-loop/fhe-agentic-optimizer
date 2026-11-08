void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext n = -t;
  Ciphertext y = -(n - b);
  y.set_output("y");
}
