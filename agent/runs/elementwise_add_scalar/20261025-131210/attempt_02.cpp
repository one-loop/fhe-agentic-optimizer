void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext t = x0 - s;
  Ciphertext y = x0 - t;
  y.set_output("y");
}
