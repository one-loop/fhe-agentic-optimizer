void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext u = b - t;
  Ciphertext y = -u;
  y.set_output("y");
}
