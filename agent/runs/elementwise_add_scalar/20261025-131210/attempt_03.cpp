void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext d = x0 + x0;
  Ciphertext t = d + s;
  Ciphertext y = t - x0;
  y.set_output("y");
}
