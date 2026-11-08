Rotating the plaintext is rejected, so the circuit stays at one plaintext multiply, one rescale and one rotation. I'll try the same computation expressed as a right rotation by `slots() - offset`. It may be normalized to the same circuit, but it is the only variant left that could change how the rotation is emitted.

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