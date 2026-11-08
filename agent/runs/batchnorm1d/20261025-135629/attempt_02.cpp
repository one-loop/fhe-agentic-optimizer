void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext u = -t;
  Ciphertext v = u - b;
  Ciphertext y = -v;
  y.set_output("y");
}
