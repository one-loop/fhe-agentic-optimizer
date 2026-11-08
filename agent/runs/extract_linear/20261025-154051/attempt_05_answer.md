The backend rejected the rotated plaintext, so the mask cannot be moved. The baseline circuit (one plaintext multiply, then one rotation) stays the cheapest valid form, and I'm resubmitting it.

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