Rotating the plaintext mask is rejected, because it counts as plaintext-plaintext evaluation. I'm going back to the baseline structure, which is already at the minimum of one plaintext multiply and one rotation.

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