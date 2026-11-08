I have no cheaper correct circuit, so I'm resubmitting the baseline circuit and expect it to be judged identical again.

- **Mask multiply:** the output needs zeros outside the window. Only a multiply by the plaintext mask can produce them, and that multiply brings one rescale with it.
- **Mask rotation:** the mask is only available unrotated, and rotating it is rejected as plaintext-plaintext arithmetic. The rotation therefore has to come after the multiply.
- **Rotation level:** the single rotation already runs at the deeper, cheaper level after the rescale.

Every alternative I tried either added operations or normalized to the same circuit, so I think the baseline is already minimal.

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