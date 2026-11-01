void fhe()
{
  Ciphertext x("x");
  Ciphertext s = square(x);
  Ciphertext a = -s;
  Ciphertext b = -a;
  Ciphertext c = -b;
  Ciphertext y = -c;
  y.set_output("y");
}
