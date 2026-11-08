void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x + x;
  Ciphertext y = t * x - square(x);
  y.set_output("y");
}
