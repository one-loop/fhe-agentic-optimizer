The baseline uses `x * x`, which is a ct-ct multiply with relinearize and rescale. The DSL counts `square(c)` as a ciphertext multiplication too, but it may lower to a dedicated squaring operation. That would be slightly cheaper because it computes fewer tensor-product terms. The circuit would differ from the baseline (square=1 rather than ct_ct_mul=1), so it can be judged as a new candidate.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```