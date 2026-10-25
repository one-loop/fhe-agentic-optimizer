void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  // One ct-pt multiply (consumes one level), then one rotation applied at the
  // reduced level after rescale. This is the minimal op count for this kernel.
  Ciphertext prod = input * mask;
  Ciphertext output = prod << offset;
  output.set_output("output");
}
