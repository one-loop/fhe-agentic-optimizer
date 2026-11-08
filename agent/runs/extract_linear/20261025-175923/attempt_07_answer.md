I'm confident the baseline circuit is the minimum, so I'm not using the last attempts on variants. The mask must be applied before the shift, which takes one ct-pt multiply and rescale. The shift takes one rotation, already placed after the rescale at the cheaper level. Extra mod-switches, split rotations or reordering would only add cost or change the result.

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