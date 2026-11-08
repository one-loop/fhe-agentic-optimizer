void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext m = x * a;
  Ciphertext nm = -m;
  Ciphertext y = b - nm;
  y.set_output("y");
}
