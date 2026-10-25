This is the same kernel for the last attempt. The 60.59 ms result is within the noise range seen on this identical program (59.61–61.21 ms). I haven't found a lower-cost version: it already uses one plaintext multiply and one rotation, and neither can be removed without changing the output.

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