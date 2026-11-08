void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = p * x0;
  y.set_output("y");
}
