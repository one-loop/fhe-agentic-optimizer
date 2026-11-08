The mask multiply is required: skipping it gave an error of 0.92, so the checker does test the full output. With one plaintext multiply and one rotation both needed, the baseline circuit is already minimal. Every variant I can build either fails correctness or adds operations. The only reordering that could help needs a plaintext-only rotation, and CHEHAB rejects that. I'm resubmitting the baseline kernel.

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