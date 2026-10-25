The operation counts are the same as the baseline: one ciphertext-plaintext multiply, one rotation, and one rescale. The 0.3 ms difference is measurement noise, so this attempt doesn't give a real improvement. I checked the alternatives. Rotating the input before the multiply would move the rotation to the top level, where it costs more. Pre-rotating the mask doesn't remove the ciphertext rotation. Skipping the mask would produce nonzero values outside the window, which fails correctness. I'm keeping the best kernel (attempt 1) unchanged.

```cpp
// Best kernel: one ciphertext-plaintext multiply (mask), then one rotation
// by slots()/4 applied after the rescale, when the ciphertext is at a lower
// level and the rotation is cheaper. The mask zeroes everything outside the
// window, so the wrapped-around tail is correct.

void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext masked = input * mask;
  Ciphertext output = masked << offset;
  output.set_output("output");
}
```