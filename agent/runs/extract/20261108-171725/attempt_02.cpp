void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Plaintext m2 = mask << offset;
  Ciphertext output = (input << offset) * m2;
  output.set_output("output");
}
