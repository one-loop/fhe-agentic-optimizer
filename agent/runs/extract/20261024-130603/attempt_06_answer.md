I'm resubmitting the same kernel. The 60.57 ms result is again within the noise band of the identical program (59.70–61.21 ms across runs). It already uses the minimum operations for this function: one plaintext multiply and one rotation. I have no further optimization to try.

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