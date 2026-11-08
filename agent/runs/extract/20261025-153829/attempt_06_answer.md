I have no new circuit to offer. The baseline's structure is the minimum I can find: one plaintext multiply with its rescale, then one rotation at the lower level.

- **Dropping the mask:** this leaves junk in the slots that should be zero, so it would fail correctness.
- **Rotating first:** multiplying the rotated input by the unrotated mask selects the wrong slots.
- **Rotating the mask:** this is rejected as plaintext-plaintext arithmetic.

I'm resubmitting the same kernel.

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