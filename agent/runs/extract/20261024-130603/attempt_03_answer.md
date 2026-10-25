This kernel is still the minimal form: one plaintext multiply and one rotation. The 59.70 ms result is within noise of the baseline, since the program is the same. I found no way to remove either operation without breaking correctness, so I'm resubmitting it unchanged.

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