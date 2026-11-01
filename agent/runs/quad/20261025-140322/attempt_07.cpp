void fhe()
{
  Ciphertext x("x");
  Ciphertext a = (x + x) - x;
  Ciphertext b = (x + x + x) - (x + x);
  Ciphertext y = a * b;
  y.set_output("y");
}
