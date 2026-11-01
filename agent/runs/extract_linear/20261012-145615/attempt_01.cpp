// Same computation as the baseline: one plaintext multiply (mask projection),
// then one rotation. Rotating after the multiply means the rotation runs at the
// already-rescaled (lower) level, which is the cheapest valid ordering.

void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext prod = input * mask;
  Ciphertext output = prod << offset;
  output.set_output("output");
}
