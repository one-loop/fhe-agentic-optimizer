void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Plaintext rm = mask << offset;
  Ciphertext output = (input << offset) * rm;
  output.set_output("output");
}
