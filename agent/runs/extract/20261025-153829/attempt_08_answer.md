I have no cheaper correct circuit. For the last attempt I'm submitting the baseline kernel again, and I expect no improvement over the baseline's 58.89 ms.

The baseline already has the minimum I can find: one plaintext multiply with its rescale, then one rotation at the lower level. The mask multiply is needed to zero the slots outside the window. The mask can't be rotated, so the rotation has to follow the multiply.

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