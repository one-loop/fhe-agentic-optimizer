That attempt took 128.82 ms, about double the baseline, because it has two multiplies. CHEHAB didn't simplify the redundant structure away.

I don't think a cheaper circuit exists for this task. Any correct evaluation of x² needs at least one ciphertext-ciphertext multiply with its relinearization and rescale. That is exactly what the baseline and the `square(x)` form already do. Without constants or plaintext inputs, I can't replace the multiply with additions or rotations. Any extra operation I add only makes the kernel slower.

Rather than spend the remaining attempts on circuits I expect to be slower, I'm resubmitting the single-multiply `square` form. It was the fastest measured, about 1% ahead of the baseline, which is below the 3% needed to count as a win.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```