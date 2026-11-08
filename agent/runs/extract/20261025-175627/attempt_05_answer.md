I can't find a cheaper circuit, so I'm resubmitting the baseline unchanged.

The function needs one mask multiply and one rotation. Every alternative I tried either needs a rotated copy of the mask, which CHEHAB rejects as a plaintext-plaintext operation, or gives wrong values. The rest only add rotations or multiplies.

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