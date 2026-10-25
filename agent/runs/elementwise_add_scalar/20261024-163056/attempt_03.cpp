void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = s + x0;
  y.set_output("y");
}
