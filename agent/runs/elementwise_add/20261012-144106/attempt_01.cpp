// Elementwise add: y = x0 + x1. This is already minimal (a single ciphertext add,
// no multiplications, rotations, or level consumption), so it is kept as is.

void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
