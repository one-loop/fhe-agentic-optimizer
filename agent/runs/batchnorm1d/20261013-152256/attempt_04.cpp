// y = x * A + B: one ciphertext-plaintext multiply and one plaintext add.
// This is the minimal evaluation; no rotations or ciphertext multiplications.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
