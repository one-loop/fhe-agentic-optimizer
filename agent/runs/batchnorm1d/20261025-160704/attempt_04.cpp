void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext z = x - x;
  z += b;
  Ciphertext y = x * a;
  y += z;
  y.set_output("y");
}
