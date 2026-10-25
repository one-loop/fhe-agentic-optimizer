I don't see a way to beat the baseline. The kernel needs one plaintext multiply to zero out everything outside the window and one rotation to move the window to slot 0. Both are required for correctness:

- **Mask multiply:** the output must be zero outside the window, so it can't be skipped.
- **Rotation:** the window has to move by `slots()/4`, and that takes a single rotation.
- **Order:** rotating first would need a rotated mask, which would be a plaintext-plaintext operation and isn't allowed. It would also run the rotation at a higher, more expensive level.

The 60.48 ms vs 61.21 ms difference between my last attempt and the baseline is run-to-run noise, since the two kernels are identical. I'm resubmitting the same minimal kernel, which already uses the fewest operations: 1 plaintext multiply and 1 rotation.

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