void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  x0 += x1;
  x0.set_output("y");
}
