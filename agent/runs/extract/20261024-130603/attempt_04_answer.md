The kernel is unchanged again: one plaintext multiply and one rotation. The 60.83 ms result differs from the best time (59.70 ms) only by run-to-run noise, because the program is identical. Neither operation can be removed without changing the output, so I'm submitting the same kernel.

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