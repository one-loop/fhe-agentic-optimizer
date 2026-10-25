void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  // Multiply by the mask first, then rotate the (already reduced-level) product.
  // Plaintext rotations are not allowed, so the rotation stays on the ciphertext.
  Ciphertext output = (input * mask) << offset;
  output.set_output("output");
}
