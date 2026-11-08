void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  Ciphertext output = (input * mask) << static_cast<int>(slots() / 4);
  output.set_output("output");
}
