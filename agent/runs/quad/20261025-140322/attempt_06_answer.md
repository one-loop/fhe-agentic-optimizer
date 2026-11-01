Attempt 5 was slower, 34.07 ms against 32.11 ms. The extra adds cost a little, and `x * t` gave no benefit.

The minimal circuit, one multiply with its relinearize and rescale, is already the baseline or `square(x)`. Every variant I can build from a single ciphertext input adds operations, and none removes any. I cannot get a circuit that is 3% faster than the baseline by restructuring, so I expect this attempt to land at the same level.

This attempt adds a zero term after the square, `square(x) + (x - x)`. It is a distinct circuit, but I expect it to be no faster.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext z = x - x;
  Ciphertext y = square(x) + z;
  y.set_output("y");
}
```