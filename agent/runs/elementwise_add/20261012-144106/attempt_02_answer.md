The result is effectively unchanged: 0.45 ms against a 0.46 ms baseline, which is timing noise. The kernel still performs a single ciphertext addition, and there is no cheaper way to produce `y = x0 + x1` under these rules, so I'm submitting the same kernel again.

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