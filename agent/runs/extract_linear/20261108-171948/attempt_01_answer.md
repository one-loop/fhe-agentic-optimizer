I don't see a way to beat the baseline here, so this kernel is the baseline unchanged and won't count as a new best.

The output needs one multiplication by the mask and one rotation by `slots()/4`. Both are already at their minimum count, and the circuit has no addition to remove.

The only reordering that could help is to rotate the input first and then multiply by a rotated mask. That would need a plaintext-only rotation, `mask << offset`, which rule 2 forbids. It would also run the rotation at the top level, where it costs more. The existing order already rotates after the multiply, at the cheaper level.

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