The baseline is already close to the minimum: one ciphertext multiplication, one relinearization, one rescale and one level. Without plaintext constants, I don't see any algebraic rewrite of `x*x` that removes any of these. Every alternative I can think of adds operations, such as extra additions or a second multiplication.

The best candidate is still the `square` form. It has a distinct circuit, with `square` counted as a separate operation, and it was faster in 4 of 5 batches, though only by about 2%. I'm resubmitting it unchanged.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```