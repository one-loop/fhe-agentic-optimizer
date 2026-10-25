// Elementwise product: one ciphertext x ciphertext multiply (depth 1, relinearized, rescaled).
// No cheaper formulation exists: any squaring-based identity needs two multiplications.

void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
