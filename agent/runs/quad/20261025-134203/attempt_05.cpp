void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x + (x - x);
  y.set_output("y");
}
