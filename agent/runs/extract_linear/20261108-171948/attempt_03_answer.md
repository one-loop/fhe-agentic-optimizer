I don't think a cheaper circuit exists for this task, so I'm resubmitting the baseline again.

The output is `(input * mask) << slots()/4`, so it needs one plaintext multiply with its rescale and one rotation. The mask is an arbitrary dense plaintext, and the baseline already spends nothing beyond that minimum.

I considered two alternatives:
- **Rotate first:** this needs a plaintext-only rotation of the mask, which attempt 2 showed CHEHAB rejects as plain-plain evaluation.
- **Split the rotation in two:** this adds a second rotation, which only costs more.

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