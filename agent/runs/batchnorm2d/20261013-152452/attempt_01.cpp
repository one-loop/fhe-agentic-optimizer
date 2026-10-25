// y = x * A + B. This is already the minimal operation count:
// one ct-pt multiply (one level) and one plaintext add.
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
