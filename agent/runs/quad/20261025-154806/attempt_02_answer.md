I don't think this circuit can get cheaper. Computing x² needs one ciphertext multiplication with its relinearize and rescale, and without plaintext inputs I can't introduce constants. I also have no way to emit a mod switch on `x` before the multiply, which would otherwise make it cheaper. Every alternative I can think of, such as scaling tricks or multiple multiplies and subtractions, adds operations.

The one remaining option is a single `square`, which was within noise of the baseline (63.59 ms vs 64.47 ms). I'm resubmitting it unchanged.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```