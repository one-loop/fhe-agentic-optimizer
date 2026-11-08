void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext nx = -x;
  Ciphertext m = nx * a;
  Ciphertext y = b - m;
  y.set_output("y");
}
