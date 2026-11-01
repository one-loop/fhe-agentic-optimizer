I still have no cheaper circuit to propose. The output needs data movement, so the rotation can't be removed, and the mask multiply can't be removed either. The rotation already runs after the rescale, at the lowest level.

Any variant I try either normalizes to the same circuit or adds work, so I'm stopping here. The baseline stays the best kernel and is the final answer.

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