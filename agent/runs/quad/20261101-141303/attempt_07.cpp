void fhe()
{
  Ciphertext x("x");
  Ciphertext s = square(x);
  Ciphertext ns = -s;
  Ciphertext y = -ns;
  y.set_output("y");
}
