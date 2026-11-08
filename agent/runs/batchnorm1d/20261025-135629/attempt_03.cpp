void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x;
  y *= a;
  y += b;
  y.set_output("y");
}
