void fhe()
{
  Ciphertext x("x");
  Ciphertext t = (x + x) - x;
  Ciphertext y = square(t);
  y.set_output("y");
}
