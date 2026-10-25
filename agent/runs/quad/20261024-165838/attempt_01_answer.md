The baseline uses `x * x`, which counts as a ct_ct_mul. The DSL says `square(c)` is also a ciphertext-ciphertext multiplication, but CHEHAB may lower it differently, for example with a cheaper relinearization path. The circuit would then differ from the baseline (square=1 instead of ct_ct_mul=1), so I'll try `square(x)`.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```