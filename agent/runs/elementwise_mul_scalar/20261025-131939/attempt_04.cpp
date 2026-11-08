void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext a = x0 + x0;
  Ciphertext b = a * s;
  Ciphertext c = x0 * s;
  Ciphertext y = b - c;
  y.set_output("y");
}
