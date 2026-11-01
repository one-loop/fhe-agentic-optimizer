I'm submitting the same kernel again. Measured times range from 0.45 to 0.46 ms across attempts, which is noise around the baseline. The program is one ciphertext addition with no multiplications, rotations, or level changes, so there is nothing left to remove.

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