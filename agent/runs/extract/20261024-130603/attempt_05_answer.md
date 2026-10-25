Same kernel again: one plaintext multiply and one rotation. The 60.03 ms result is within noise of the earlier runs (59.70–61.21 ms), since the program hasn't changed. I haven't found a way to drop either operation without changing the output, so there's nothing left to change.

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