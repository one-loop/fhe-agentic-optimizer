The baseline is still the minimum: no correct circuit for this function is cheaper.

- **Multiply:** the mask is a runtime plaintext, so there must be one plaintext multiply with its rescale.
- **Rotation:** the output is shifted, and a plaintext cannot be rotated in this DSL (attempt 2). So there must be one ciphertext rotation.
- **Order:** rotating after the rescale is already the cheaper choice.

Any different circuit would add operations and run slower. I'm submitting the optimal kernel unchanged.

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