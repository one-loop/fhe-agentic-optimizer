void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int n = static_cast<int>(slots());
  const int offset = n / 4;
  // Multiply first so the rotation runs after the rescale, at a cheaper level.
  Ciphertext masked = input * mask;
  // Right rotation by 3n/4 is the same cyclic shift as left rotation by n/4.
  Ciphertext output = masked >> (n - offset);
  output.set_output("output");
}
