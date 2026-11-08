void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input * mask) >> (static_cast<int>(slots()) - offset);
  output.set_output("output");
}
