The mask is a plaintext input, so it can't be rotated (that needs a plaintext-plaintext operation), and rotating the input first would apply the wrong mask. The baseline order (plaintext multiply, then rotate at the lower level) is already close to minimal: one ct-pt multiply, one rescale, one rotation.

For this attempt I'll express the same left rotation by `slots()/4` as a right rotation by `slots() - slots()/4`. CHEHAB probably normalizes it to the same circuit, but it is a valid, safe variant to measure.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext prod = input * mask;
  Ciphertext output = prod >> (static_cast<int>(slots()) - offset);
  output.set_output("output");
}
```