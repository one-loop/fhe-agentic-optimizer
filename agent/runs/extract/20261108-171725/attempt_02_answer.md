The baseline circuit is fixed by the rotation and the mask multiply. This attempt changes its structure: rotate the ciphertext first, then multiply by the mask rotated by the same amount. The window then lines up with the rotated mask. If CHEHAB accepts rotating a plaintext, the circuit differs from the baseline's.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Plaintext m2 = mask << offset;
  Ciphertext output = (input << offset) * m2;
  output.set_output("output");
}
```