I'm submitting the same kernel for the last time. The result is 0.45 ms, the same as the best earlier attempt, and the baseline is 0.46 ms, so the gap is noise. The kernel performs one ciphertext addition and nothing else, which is the minimum for `y = x0 + x1`. Any change would only add operations.

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