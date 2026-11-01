// y = x * A + B: one ciphertext-plaintext multiply (depth 1) plus one plaintext add.
// This is the minimal evaluation: no rotations, no ciphertext-ciphertext multiplies.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
