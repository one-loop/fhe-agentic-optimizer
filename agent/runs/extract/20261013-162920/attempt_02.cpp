// Best kernel: one ciphertext-plaintext multiply (mask), then one rotation
// by slots()/4 applied after the rescale, when the ciphertext is at a lower
// level and the rotation is cheaper. The mask zeroes everything outside the
// window, so the wrapped-around tail is correct.

void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext masked = input * mask;
  Ciphertext output = masked << offset;
  output.set_output("output");
}
