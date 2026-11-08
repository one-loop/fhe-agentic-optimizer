I found no kernel that beats the baseline. Seven attempts ended up with the baseline's circuit, so none could be timed as faster. This final submission is the baseline structure again.

The baseline is already the minimum I can express. It uses one ct-pt multiply with its rescale, then one rotation at the lower level. The mask is a plaintext input and can't be rotated, so the multiply can't move after the shift. Splitting the rotation, or adding mod-switches or alignments, only adds work.

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