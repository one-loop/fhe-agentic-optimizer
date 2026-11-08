I don't have a valid circuit that beats the baseline, so I'm resubmitting it unchanged.

Computing the same function takes one plaintext multiply (to zero everything outside the window) and one rotation. Every cheaper variant I could think of fails:

- **Rotate first, then multiply:** this needs the mask rotated by the same offset. Rotating the mask is a plaintext-plaintext operation, which CHEHAB rejected in attempt 3.
- **Drop the mask:** the dense case would then leave wrapped data outside slots 0 to N/2, where the expected output is zero.
- **Split the rotation into smaller steps:** this adds rotations, which are the expensive part.

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