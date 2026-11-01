I don't have a cheaper circuit. The plaintext-rotation idea was rejected by the backend, since a rotated mask is a plaintext-only operation. Any other way to get the window to slot 0 either costs extra rotations or skips the mask, which changes the result. The baseline is already minimal at one plaintext multiply, one rescale and one rotation, with the rotation done at the lowest level. I'm resubmitting the same kernel, which will not count as an improvement.

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