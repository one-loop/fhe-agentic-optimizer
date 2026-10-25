void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  // Alternative ordering: rotate the input first, then multiply by the
  // rotated mask so that the product lands in the same slots as before.
  Ciphertext rotated = input << offset;
  Ciphertext output = rotated * (mask << offset);
  output.set_output("output");
}
