void fhe()
{
  Ciphertext x("x");
  Ciphertext z = x - x;
  Ciphertext y = square(x) + z;
  y.set_output("y");
}
