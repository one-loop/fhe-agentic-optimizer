void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  // Rotate first, then mask with the correspondingly rotated mask:
  // ((input << k) * (mask << k))[i] = input[i+k] * mask[i+k] = ((input*mask) << k)[i]
  Ciphertext rotated = input << offset;
  Plaintext rmask = mask << offset;
  Ciphertext output = rotated * rmask;
  output.set_output("output");
}
