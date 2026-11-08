I have no further idea for a cheaper circuit. The single plaintext multiply and single rotation are both required, and I can't reorder them without a rotated plaintext mask, which the DSL doesn't allow. I'm keeping the baseline as the best kernel.

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