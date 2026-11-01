The new best is 29.09 ms (attempt 6), but it's still within the 29.09–29.64 ms noise band for an identical operation graph. I'm keeping the same kernel as final.

```cpp
// Final kernel: one ct-pt multiply by the mask, then one rotation by slots()/4.

void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input * mask) << offset;
  output.set_output("output");
}
```