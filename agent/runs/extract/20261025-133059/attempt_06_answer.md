I'm resubmitting the baseline circuit. Every correct alternative I can construct needs a rotated plaintext mask or extra operations. Neither can beat one plaintext multiply followed by one rotation.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  Ciphertext output = (input * mask) << static_cast<int>(slots() / 4);
  output.set_output("output");
}
```