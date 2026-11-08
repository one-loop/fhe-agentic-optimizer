void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext n = -x0;
  Ciphertext m = n - p;
  Ciphertext y = -m;
  y.set_output("y");
}
