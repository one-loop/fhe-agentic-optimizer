void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext s = x0 + p;
  Ciphertext y = s;
  y.set_output("y");
}
