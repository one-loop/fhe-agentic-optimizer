void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext d = x0 + x0;
  Ciphertext y = d * p - x0 * p;
  y.set_output("y");
}
