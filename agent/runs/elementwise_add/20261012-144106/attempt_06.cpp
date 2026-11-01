// Elementwise add: y = x0 + x1. Already minimal (one ciphertext add, no
// multiplications, rotations, or level consumption); no further change possible.

void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
