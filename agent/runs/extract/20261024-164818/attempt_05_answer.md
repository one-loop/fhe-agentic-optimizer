I still can't find a valid circuit with fewer operations than the baseline, so I'm resubmitting it unchanged.

Every correct form needs `input * mask` followed by a rotation by `slots()/4`. The mask is a plaintext input that can't be rotated, and the zeros outside the window can only come from multiplying by it. Reordering the two operations moves the rotation to a higher level, where it costs more. I'd rather keep a correct, known-timed kernel than submit a variant that changes the result.

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