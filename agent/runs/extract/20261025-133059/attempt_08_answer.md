I found no correct circuit cheaper than the baseline, so this last attempt is the baseline again.

Rotating first would need the mask rotated as well. The backend rejects that because it counts it as plaintext-plaintext evaluation. Masking first and then rotating takes one plaintext multiply with its rescale and one rotation. That is the minimum for this function.

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