The kernel needs one ciphertext-plaintext multiply (to apply the mask, including the zeros outside the window) and one rotation (to move the window to slot 0). Both are unavoidable, and the baseline already performs them in the cheaper order: multiply first, so the rotation runs after the rescale at a lower level. Rotating first would put the rotation at the top level, which costs more. Rotating the mask would need a plaintext-side shift, which doesn't remove the ciphertext rotation anyway. So I'm keeping the baseline's operation sequence and only making the offset explicit.

```cpp
// Minimal sequence: one ciphertext-plaintext multiply, then one rotation
// performed after the rescale (lower level, cheaper). The mask zeroes the
// slots outside the window, so the wrapped-around tail is correct.

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