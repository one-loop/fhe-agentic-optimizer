// y = x * A + B: one plaintext multiply (depth 1) and one plaintext add.
// This is already the minimal operation set: no rotations, no ciphertext-ciphertext multiplies.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
