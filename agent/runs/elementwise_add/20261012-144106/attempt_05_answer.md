I'm submitting the same kernel once more. Across attempts the time stays between 0.45 and 0.46 ms, which is measurement noise around the baseline. The kernel already does the minimum: one ciphertext addition, with no multiplications, rotations, or level consumption. Any alternative would add operations and make it slower, so I'm not changing it.

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