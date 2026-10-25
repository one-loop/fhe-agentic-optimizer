// y = x * A + B. Minimal form: one ct-pt multiply (one level) plus one plaintext add.
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
