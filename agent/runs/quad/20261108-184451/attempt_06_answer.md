I have no cheaper circuit to propose. `y = x*x` already uses the minimum for CKKS without plaintext constants: one ciphertext multiplication, one relinearization, one rescale and one level. Any structurally different form I can build adds operations, so it would be slower.

The best result remains the `square(x)` kernel from attempt 1. It was correct and measured about 2% faster than the baseline (5.99 ms vs 6.10 ms), which is below the 3% needed for confirmation. I'm not spending the remaining attempts on resubmissions that the judge would reject as duplicates.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```