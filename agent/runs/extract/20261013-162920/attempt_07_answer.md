I'm keeping the same kernel. Attempt 6 measured 61.89 ms with the same operation counts as every other attempt, so the spread across six runs (59.3 to 61.9 ms) is run-to-run noise. The work is already the minimum: one ciphertext-plaintext multiply to apply the mask and one rotation to bring the window to slot 0.

Final kernel (attempt 1, best measured at 59.29 ms):

```cpp
// Final kernel: one ciphertext-plaintext multiply by the mask, then one
// rotation by slots()/4 after the rescale (lower level, cheaper). The mask
// zeroes everything outside the window, so the wrapped-around tail is correct.

void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext masked = input * mask;
  Ciphertext output = masked << offset;
  output.set_output("output");
}
```