I have no valid cheaper circuit, so I'm not going to submit a variant that is correct only by accident or slower. The mask must be applied at the original slot positions because I can't rotate a plaintext. Rotating the ciphertext first would then need a second multiply or extra rotations. The baseline already has the minimal operation set: one plaintext multiply with its rescale, and one rotation at the lowest level. I'm keeping it as my answer.

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