Rotating a plaintext is rejected, and the mask can't be pre-rotated. The remaining cheapest form is multiply, then rotate, which is the baseline. As a last variation I'll express the rotation as a right rotation by `slots() - offset`, which is mathematically identical and may lower differently.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext masked = input * mask;
  Ciphertext output = masked >> (static_cast<int>(slots()) - offset);
  output.set_output("output");
}
```