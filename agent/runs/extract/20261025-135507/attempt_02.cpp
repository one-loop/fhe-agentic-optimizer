void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Plaintext rmask = mask << offset;
  Ciphertext output = (input << offset) * rmask;
  output.set_output("output");
}
