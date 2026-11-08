void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext z = x0 - x0;
  Ciphertext y = x0 * p + z * p;
  y.set_output("y");
}
