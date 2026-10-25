I'm stopping the search here. The last three attempts all measured between 59.3 and 60.0 ms with identical operation counts, so the differences are run-to-run noise. Nothing I can change within the CKKS rules reduces the work below one ciphertext-plaintext multiply and one rotation.

Attempt 1 is the best measured kernel (59.29 ms, correct on all cases), so I'm submitting it as the final answer:

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