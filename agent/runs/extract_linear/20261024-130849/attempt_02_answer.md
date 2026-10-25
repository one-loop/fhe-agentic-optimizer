The previous attempt confirmed the baseline is already minimal in operation count, so this attempt tries a different ordering. I rotate the plaintext mask instead of the ciphertext product, then rotate the input and multiply. This might let CHEHAB schedule the work differently, though it may not be accepted for plaintext inputs.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext r = input << offset;
  Plaintext m = mask << offset;
  Ciphertext output = r * m;
  output.set_output("output");
}
```