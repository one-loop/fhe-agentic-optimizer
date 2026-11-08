void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  // Mask first (ct-pt multiply + rescale), then rotate at the lower, cheaper level.
  Ciphertext output = (input * mask) << offset;
  output.set_output("output");
}
