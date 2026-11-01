I found no cheaper circuit in any of the attempts, so the last attempt is the baseline circuit again. The output needs `input[i+N/4] * mask[i+N/4]`, and the mask can only be applied at its original slot positions. The one alternative, a rotated mask, is a plaintext-plaintext operation that the backend rejects (attempt 2). That leaves one plaintext multiply with its rescale and one rotation, which is what the baseline already does.

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