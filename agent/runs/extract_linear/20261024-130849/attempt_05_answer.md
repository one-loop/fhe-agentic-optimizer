The 65.94 ms result for an identical kernel shows run-to-run noise of several milliseconds, so the differences between my attempts aren't meaningful. The kernel is already at one plaintext multiply and one rotation, and I can't reduce that further, so I'm resubmitting the best kernel.

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