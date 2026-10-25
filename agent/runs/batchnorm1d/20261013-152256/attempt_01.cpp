// y = x * A + B, the minimal evaluation: one ciphertext-plaintext multiply
// (one level) and one plaintext-vector add. No rotations or ciphertext
// multiplications are needed, so this is already the cheapest form.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext y = t + b;
  y.set_output("y");
}
