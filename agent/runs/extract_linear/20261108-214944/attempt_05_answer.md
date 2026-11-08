Attempt 4 confirms it: the baseline uses the fewest operations this function allows, and no correct circuit is cheaper.

- **Multiply:** the mask is a runtime plaintext, so one ciphertext × plaintext multiply and its rescale are required.
- **Rotation:** the output is shifted against both operands, and a plaintext cannot be rotated in this DSL (attempt 2). So one ciphertext rotation is required.
- **Order:** rotating after the rescale is already the cheaper position.

Any variant either gives the same circuit, which is not timed, or adds operations and is slower. Rotating before an unshifted mask computes the wrong function. I recommend ending the search here; the baseline is the best kernel:

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