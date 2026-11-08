I still can't find a correct circuit with fewer operations. The mask must be applied before the rotation, since it can't be rotated as a plaintext. The multiply needs one rescale and the rotation is one key-switch, and neither can be removed or merged. I'm returning the baseline circuit again.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext masked = input * mask;
  Ciphertext output = masked << offset;
  output.set_output("output");
}
```