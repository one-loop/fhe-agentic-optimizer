That was a duplicate again, and I shouldn't have resubmitted it. The result tells me nothing new.

I still have no way to remove the single multiply, relinearization and rescale. The only new circuits I can build keep that one multiply and add extra operations. Those can't beat the baseline, but I'll test whether CHEHAB handles them differently. This one is `x*x` plus a cancelling `x - x` term. If CHEHAB simplifies it back to the original, the circuit will be reported as identical. If it doesn't, it will be slightly slower.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x + (x - x);
  y.set_output("y");
}
```