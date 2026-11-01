void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext d = x0 + x0;
  Ciphertext y = d * s - x0 * s;
  y.set_output("y");
}
