void fhe()
{
  Ciphertext x("x");
  Ciphertext nx = -x;
  Ciphertext y = -(nx * x);
  y.set_output("y");
}
