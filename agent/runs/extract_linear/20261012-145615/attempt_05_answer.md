The result is 29.18 ms, within noise of the best (29.11 ms) and the baseline (29.13 ms). The kernel is the same as attempts 1–3, so I'm resubmitting it unchanged.

```cpp
// Best kernel: one plaintext multiply (mask projection), then one rotation
// applied after the rescale, so the rotation runs at the lower level.

void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext prod = input * mask;
  Ciphertext output = prod << offset;
  output.set_output("output");
}
```