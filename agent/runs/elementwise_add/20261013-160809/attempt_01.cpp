// Elementwise add: y = x0 + x1. Already minimal (one ciphertext add, depth 0).

void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
