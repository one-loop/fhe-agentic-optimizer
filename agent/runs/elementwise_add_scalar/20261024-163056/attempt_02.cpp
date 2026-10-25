void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0;
  y += s;
  y.set_output("y");
}
