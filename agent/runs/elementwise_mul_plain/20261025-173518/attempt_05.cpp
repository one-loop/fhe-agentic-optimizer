void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext a = x0 * p;
  Ciphertext y = a + a - a;
  y.set_output("y");
}
