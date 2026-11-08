I still have no valid, cheaper circuit, so I'm leaving the baseline kernel as the answer.

The baseline already uses the minimum for this function: one plaintext multiply (with its rescale) and one rotation, at a single level. Everything that changes the circuit either breaks CKKS rule 2 or gives wrong values:

- **Rotated mask:** `mask << k` is a plaintext-plaintext operation and is rejected by CHEHAB.
- **Rotate first, then mask with the original mask:** this keeps the wrong slots.
- **Dropping the mask:** this leaves wrapped data in the output where zeros are expected.
- **Splitting the rotation into two half-offset rotations:** this only adds a rotation, which is the costliest part.

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