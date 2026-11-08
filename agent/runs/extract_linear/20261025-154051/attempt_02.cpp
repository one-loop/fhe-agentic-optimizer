void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext t = input * mask;
  Ciphertext output = (t << (offset / 2)) << (offset - offset / 2);
  output.set_output("output");
}
