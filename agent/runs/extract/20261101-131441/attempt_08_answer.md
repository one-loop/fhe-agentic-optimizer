For this last attempt I'm again returning the baseline, because I couldn't find a cheaper correct kernel. Only attempt 1 tried a different structure; attempts 2 to 7 resubmitted this same circuit, so none of them could win.

The baseline is one ct×pt multiply, its rescale, and one rotation, and I believe that is the minimum:
- **The multiply is required.** The output must be zero outside the window. That needs a multiply that differs from slot to slot, and `mask` is the only constant that can supply it.
- **The rotation is required.** The window has to move from slot n/4 to slot 0. Rotating the mask instead of the ciphertext was rejected as a plaintext-only operation.
- **The order is already the cheaper one.** The rotation runs after the rescale, on the lower level, where it costs less.

Any other structure either uses an operation CHEHAB rejects or adds operations, so it would be slower.

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