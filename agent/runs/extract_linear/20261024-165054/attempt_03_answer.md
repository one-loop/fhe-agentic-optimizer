I don't see a cheaper circuit for this task. Any different structure would add operations:

- **Rotating first:** this needs a rotated mask, which is a plaintext-plaintext operation and is rejected.
- **Splitting the rotation:** two smaller rotations cost more than one.
- **Extra mask handling:** any added multiply, add or alignment only increases the cost.

The baseline already has the minimum: one ct-pt multiply with rescale, then a single rotation at the reduced level. I'm resubmitting it unchanged as the best kernel.

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