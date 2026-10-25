Attempt 7 measured 59.24 ms, a hair under attempt 1's 59.29 ms. That gap is within the noise seen across runs, so the two kernels are equivalent. The code is the same computation either way, so I'm submitting it as the final answer.

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