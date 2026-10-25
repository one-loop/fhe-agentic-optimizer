// y = x * A + B: one ciphertext-plaintext multiply (one level) followed by
// one plaintext-vector add. This is the minimal evaluation of the task.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext y = t + b;
  y.set_output("y");
}
