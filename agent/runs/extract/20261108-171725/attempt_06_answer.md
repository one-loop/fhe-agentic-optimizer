I have no new circuit to propose. After five attempts, the cheapest correct circuit I can find is the baseline's: one plaintext multiply (with its rescale) and one rotation. Every change I tried either produced the same circuit or was rejected. The only structurally different idea, rotating the mask as a plaintext, is not supported by CHEHAB. Dropping the mask multiply leaves unmasked data in the upper half, so it fails the dense test. I am resubmitting the baseline kernel, which cannot beat the best.

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