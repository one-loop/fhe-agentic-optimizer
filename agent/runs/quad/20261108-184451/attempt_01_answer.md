The baseline uses `x * x`, which is a ciphertext-ciphertext multiply. Using `square(x)` is the same computation, but it is counted as a different operation, so the circuit differs from the baseline. Squaring needs fewer polynomial products inside the multiply, and relinearization and rescale stay the same. It may be slightly faster.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```