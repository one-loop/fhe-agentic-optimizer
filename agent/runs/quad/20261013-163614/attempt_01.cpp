// y = x * x using the dedicated squaring operator (single ct-ct square, one relin + rescale).

void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
