// Elementwise product: a single ciphertext x ciphertext multiply (depth 1).
// This is already the minimal operation count: one ct-ct mul (+relin, rescale), no rotations.

void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
