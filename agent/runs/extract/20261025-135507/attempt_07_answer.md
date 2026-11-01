I have no new idea that is both valid and cheaper. Every alternative I can build needs either a rotated mask, which the backend rejects, or more rotations and multiplies. The baseline circuit stays my answer.

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