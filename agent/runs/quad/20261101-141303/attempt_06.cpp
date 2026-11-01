void fhe()
{
  Ciphertext x("x");
  Ciphertext nx = -x;
  Ciphertext y = square(nx);
  y.set_output("y");
}
