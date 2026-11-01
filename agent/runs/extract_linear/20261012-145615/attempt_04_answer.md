Attempt 3 measured 29.42 ms, slower than the best (29.11 ms), and it is the same kernel as attempts 1–2 with only timing noise. I'm resubmitting the best kernel. Its one multiply and one rotation are the minimum this function needs, so I don't expect a further speedup.

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