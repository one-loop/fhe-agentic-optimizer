The baseline is already near the minimum for this circuit: one plaintext multiply and one rotation. I can't rotate the mask, because that would be plaintext-plaintext work. Rotating the input first would need that rotated mask.

For this attempt I'm testing a different circuit that rotates the product in two half-steps. It is probably not faster, since it uses two rotations instead of one, but it will show whether the rotation cost behaves differently at the lower level.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext t = input * mask;
  Ciphertext output = (t << (offset / 2)) << (offset - offset / 2);
  output.set_output("output");
}
```