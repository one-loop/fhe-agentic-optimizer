I don't see a cheaper circuit for this function. The output is `rotate(input * mask, slots/4)`, which needs one plaintext multiply with its rescale and one rotation. Both are required, and the order already puts the rotation at the cheaper, lower level.

I considered two alternatives:
- **Rotate first, then multiply by a rotated mask:** this needs a plaintext rotation, which the DSL doesn't allow without a ciphertext operand. The rotation would also run at the more expensive top level.
- **Split the rotation into several smaller ones:** this adds key-switches, so it costs more.

I'm resubmitting the baseline structure, since any change I can find only adds operations.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input * mask) << offset;
  output.set_output("output");
}
```