void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext m = x * a;
  Ciphertext y = m - (-(m - m) - b);
  y.set_output("y");
}
