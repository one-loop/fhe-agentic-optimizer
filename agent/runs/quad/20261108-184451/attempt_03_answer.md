I don't have a structurally different circuit that is cheaper. `y = x*x` already needs the minimum: one ciphertext multiplication, one relinearization, one rescale and one level. CKKS forbids numeric constants and plaintext-only arithmetic, so there is no cheaper identity such as `(x+x)²/4`. The alternatives I can think of only add additions or multiplications.

The `square(x)` kernel from attempt 1 is therefore my final answer. It was correct and about 2% faster than the baseline (5.99 ms vs 6.10 ms), though that was below the 3% confirmation threshold.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```