I'm submitting the same kernel again. It measures 0.45 ms, the same as the best earlier attempt, and the baseline is 0.46 ms, so the difference is noise. The program is a single ciphertext addition with no multiplications, rotations, or level changes, which is the minimum possible work for `y = x0 + x1`.

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