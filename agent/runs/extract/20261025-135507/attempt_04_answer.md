I still have no cheaper circuit. Output slot i has to equal `input[i+N/4] * mask[i+N/4]`. Getting the mask to line up with a pre-rotated input needs a plaintext rotation, which the backend rejects. Any workaround adds rotations or plaintext multiplies. The baseline, with one plaintext multiply, one rescale and one rotation at the lowest level, is the minimum I can reach in this DSL, so I'm submitting it again.

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