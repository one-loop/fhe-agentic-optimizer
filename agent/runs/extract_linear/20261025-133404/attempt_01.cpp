void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext r = input << offset;
  Ciphertext output = r * (mask << offset);
  output.set_output("output");
}
