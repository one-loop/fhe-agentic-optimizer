I have nothing new that is valid. The baseline remains the best circuit I can find, so I'm leaving it as my final answer and not spending the remaining attempts on rewrites that either fail or add operations.

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