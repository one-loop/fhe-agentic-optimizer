The baseline already uses the minimum: one plaintext multiply, one rescale and one rotation. The only structurally different circuit I can think of is to rotate the ciphertext first and multiply by a rotated mask, so `(input << k) * (mask << k)`. This may not be cheaper, because the ciphertext rotation then runs at the top level, and the rotated plaintext mask may need an extra encoding. It is the one alternative circuit available, so I'm testing it.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Plaintext rmask = mask << offset;
  Ciphertext output = (input << offset) * rmask;
  output.set_output("output");
}
```