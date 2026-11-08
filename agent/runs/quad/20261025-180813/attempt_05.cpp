void fhe()
{
  Ciphertext x("x");
  Ciphertext z = x - x;
  Ciphertext y = x * x + z;
  y.set_output("y");
}
