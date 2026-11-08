This is my last attempt, and I still have no valid circuit that beats the baseline, so I'm resubmitting it unchanged.

I tried to avoid the mask multiply or the rotation, and every route fails:

- **Rotate first, then mask:** after rotating, the garbage sits in slots N/2 to N. The given mask covers slots N/4 to 3N/4, so it cannot clear exactly those slots.
- **Pre-rotated mask:** rotating the mask is a plaintext-plaintext operation, and CHEHAB rejects it.
- **No mask at all:** this leaves wrapped data where the expected output is zero.

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