No correct circuit cheaper than the baseline exists, so I'm submitting it again rather than spending attempts on slower variants.

The function needs at least three operations:
- **One plaintext multiply and its rescale**, because the mask is a runtime input.
- **One ciphertext rotation**, because the output is shifted and the DSL cannot rotate a plaintext (attempt 2).

The baseline has exactly those three operations, with the rotation in the cheaper position after the rescale. Any other correct circuit adds operations.

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