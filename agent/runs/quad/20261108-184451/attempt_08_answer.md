The `square(x)` circuit remains my final answer. In attempt 1 it was correct and about 2% faster than the baseline (5.99 ms vs 6.10 ms), but that is below the 3% needed for confirmation. I have no cheaper circuit for this last attempt.

`y = x*x` already uses the minimum for CKKS without plaintext constants: one ciphertext multiplication, one relinearization, one rescale and one level. Every structurally different form I can construct adds operations. For example, `(x+x)*x - x*x` equals `x²` but needs two multiplications. Submitting one of those would only be slower, so I'm giving the best kernel again.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```