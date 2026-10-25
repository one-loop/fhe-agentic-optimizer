The baseline computes (input * mask) << offset, which costs one plaintext multiply and one rotation. Rotating first and then multiplying would need the mask rotated, which can't be done on plaintext inputs without a ciphertext operand. The rotation count is already minimal, so one rotation and one multiply is probably the floor.

A cheaper variant is to rotate the input at the deepest level before the multiply, but both orders cost the same: the rotation at level 0 costs ~20 ms either way. Rotating after the multiply happens at a lower level (the ciphertext has one fewer modulus), so it is slightly cheaper. The baseline already does this.

I can't reduce the operation count, so I'm resubmitting the same structure.

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