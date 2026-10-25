// y = x * x via the dedicated squaring operator: one ct-ct square, one relin + rescale.
// This is the minimum work for quadratic activation (a single ciphertext multiplication).

void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
