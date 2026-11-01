// Final kernel: one ct-pt multiply by the mask, then one rotation by slots()/4.
// Both operations are required; the rotation runs after the multiply at the lower level.

void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input * mask) << offset;
  output.set_output("output");
}
