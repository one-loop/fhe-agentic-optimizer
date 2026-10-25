void fhe()
{
  Ciphertext x("x");
  Ciphertext a = x + x;
  Ciphertext y = a * x - x * x;
  y.set_output("y");
}
