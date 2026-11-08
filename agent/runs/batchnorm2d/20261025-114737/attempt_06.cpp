void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext nx = -x;
  Ciphertext m = nx * a;
  Ciphertext k = m - b;
  Ciphertext y = -k;
  y.set_output("y");
}
