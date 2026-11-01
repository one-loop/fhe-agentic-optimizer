void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = a * x;
  Ciphertext y = b + t;
  y.set_output("y");
}
