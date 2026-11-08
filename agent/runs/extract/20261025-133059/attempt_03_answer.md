Plaintext rotation is not supported, so the mask can't be pre-rotated and the ciphertext has to be masked before it is rotated. The baseline circuit of one plaintext multiply and one rotation, with the rotation at the already-reduced level, appears to be the minimum. I can't find a cheaper circuit, so I'm returning it unchanged.

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