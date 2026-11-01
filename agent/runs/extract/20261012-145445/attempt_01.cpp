// Minimal op count: one ct-pt multiply by the mask, then one rotation.
// The rotation stays after the multiply so it runs at the already-reduced level.

void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext masked = input * mask;
  Ciphertext output = masked << offset;
  output.set_output("output");
}
