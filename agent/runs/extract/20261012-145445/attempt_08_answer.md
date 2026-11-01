I'm keeping the same kernel. Attempt 7 measured 29.12 ms, inside the same noise band as every other run of this identical graph (29.09–29.64 ms). The operation counts are fixed at one ct-pt multiply, one rotation and one rescale, and both the multiply and the rotation are required.

```cpp
// Final kernel: one ct-pt multiply by the mask, then one rotation by slots()/4.

void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input * mask) << offset;
  output.set_output("output");
}
```