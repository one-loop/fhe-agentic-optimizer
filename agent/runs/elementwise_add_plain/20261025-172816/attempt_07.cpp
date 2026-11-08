void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext t = x0 + p;
  Ciphertext y = t + x0 - x0;
  y.set_output("y");
}
