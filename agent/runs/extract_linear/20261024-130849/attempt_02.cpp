void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext r = input << offset;
  Plaintext m = mask << offset;
  Ciphertext output = r * m;
  output.set_output("output");
}
