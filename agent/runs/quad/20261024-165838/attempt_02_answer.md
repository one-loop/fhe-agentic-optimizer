`x²` needs at least one ciphertext-ciphertext multiplication with relinearization and rescale. There is no plaintext input to exploit and no rotation to save, so I don't see a cheaper circuit than the baseline. The only variant I can think of that changes the circuit is `(x + x) * x - x * x`, which uses two multiplications and is strictly worse. I'll resubmit the minimal form below as a sanity check, and I don't expect it to win.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```