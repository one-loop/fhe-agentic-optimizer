void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext nx = -x;
  Ciphertext t = nx * a;
  Ciphertext u = t - b;
  Ciphertext y = -u;
  y.set_output("y");
}
