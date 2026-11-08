The baseline is already at the minimum for this function, so I don't expect any kernel to beat it.

1. **The multiply is required.** The mask is an unknown plaintext input, so there must be at least one ciphertext × plaintext multiply and its rescale.
2. **The rotation is required.** The window has to move by slots()/4, which takes at least one rotation.
3. **The order is already the cheap one.** Rotating after the rescale means the rotation runs on a ciphertext with one fewer prime. Rotating first would make the rotation more expensive.
4. **Rotating the mask instead isn't allowed.** It would be a plaintext-only operation, which rule 2 forbids.

So the baseline's 1 multiply, 1 rescale and 1 rotation at 1 level is the floor. Rewriting the shift as `>> 3*slots()/4` is the same single rotation and gives the same circuit, so it cannot win either. Below is the minimal kernel, which is the baseline itself.

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