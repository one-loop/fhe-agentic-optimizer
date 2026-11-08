void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int half = static_cast<int>(slots() / 8);
  Ciphertext m = input * mask;
  Ciphertext output = (m << half) << half;
  output.set_output("output");
}
