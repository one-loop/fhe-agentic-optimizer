void fhe()
{
  Ciphertext x("x");
  Ciphertext t = (x + x) - x;
  Ciphertext y = x * t;
  y.set_output("y");
}
