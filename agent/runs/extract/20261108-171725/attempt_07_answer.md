I have no further idea for a cheaper circuit. The baseline has the minimum I can see: one plaintext multiply, one rescale and one rotation, with the rotation already applied after the rescale. Every rewrite I tried either collapsed to the same circuit or was rejected for plaintext-only arithmetic. I'm submitting the baseline kernel once more and expect it to be rejected as identical again.

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