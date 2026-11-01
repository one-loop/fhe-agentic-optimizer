Attempt 2 failed because CHEHAB does not allow rotating a plaintext, so the rotation cannot be moved onto the mask. That leaves no cheaper circuit than the baseline:

- **The multiply is required.** `mask` is an arbitrary runtime plaintext, so the one ct-pt multiply cannot be removed, and its rescale comes with it.
- **The rotation is required.** Moving the window needs exactly one ciphertext rotation. It already runs after the rescale, at the cheapest level.
- **Every other legal structure costs more.** Rotating before the multiply puts the rotation at a higher level. Splitting the mask or rotation only adds operations.

No different circuit can be faster than the best, so I'm returning the best kernel. More attempts would only produce circuits that cost more.

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