// y = x^2 with a single ciphertext squaring (depth 1, no rotations).

void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
