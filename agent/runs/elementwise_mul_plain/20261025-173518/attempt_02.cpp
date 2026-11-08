void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext a = x0 + x0;
  Ciphertext b = a * p;
  Ciphertext c = x0 * p;
  Ciphertext y = b - c;
  y.set_output("y");
}
