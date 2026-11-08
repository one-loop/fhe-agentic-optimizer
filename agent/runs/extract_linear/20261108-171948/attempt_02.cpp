void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext rotated = input << offset;
  Plaintext pm = mask << offset;
  Ciphertext output = rotated * pm;
  output.set_output("output");
}
