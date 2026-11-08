I still have no cheaper correct circuit. I'm submitting the same kernel again, which the judge will treat as identical to the baseline.

The baseline already uses one plaintext multiply, one rescale and one rotation, and I found no way to remove or cheapen any of them:

- **Multiply:** the multiply by the runtime mask is needed to zero the slots outside the window.
- **Rotation placement:** it has to come after the multiply, because the mask can't be rotated.
- **Rotation level:** it already runs at the cheaper level after the rescale.

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