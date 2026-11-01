A single ciphertext add is already the minimum for this function. Every alternative I can think of adds operations, and with no multiplications, rotations or rescales there is nothing left to remove. A circuit with fewer operations than one add would have to skip the add entirely, which cannot compute x0 + x1.

I'm resubmitting the same kernel. It will most likely be rejected again as identical to the baseline, but I have no cheaper valid circuit to offer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```