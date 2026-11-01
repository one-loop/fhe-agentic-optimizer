I'm keeping the same kernel. Attempt 5 measured 29.17 ms, matching the baseline, and the spread across identical runs is roughly 29.1–29.6 ms. Nothing in the operation counts has changed across six attempts, so further attempts would only re-measure noise.

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