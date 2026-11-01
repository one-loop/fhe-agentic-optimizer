I'm submitting the same kernel again. The last run measured 0.46 ms, the same as the baseline, and the earlier 0.45 ms was within noise. The kernel is a single ciphertext addition with no multiplications, rotations, or level changes, so no change to the program can reduce its work.

```cpp
// Elementwise add: y = x0 + x1. Already minimal (one ciphertext add, no
// multiplications, rotations, or level consumption); no further change possible.

void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```