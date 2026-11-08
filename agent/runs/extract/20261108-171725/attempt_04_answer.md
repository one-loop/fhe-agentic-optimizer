I don't have a correct kernel with a cheaper circuit. Each alternative I considered either changes the result or adds operations:

- Rotating first and then multiplying by `mask` gives wrong values. The mask would have to be rotated too, and CHEHAB rejects plaintext-only operations (attempt 2).
- Dropping the mask multiply leaves unmasked data in the upper half of the output. The dense random test would catch it.
- Splitting the rotation into several smaller rotations adds work.

The baseline circuit is one plaintext multiply with its rescale, then one rotation. I don't see a way to get below that, so I'm resubmitting it unchanged and expect it to be rejected as identical.

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