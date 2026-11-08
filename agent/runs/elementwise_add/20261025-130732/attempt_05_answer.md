The baseline is already a single ciphertext add, and the output depends on both inputs, so at least one operation is required. Every alternative I can think of (rotations, multiplications, extra alignments) only adds cost. I have no cheaper circuit to offer, so I'm returning the same kernel again. It can't win under the judging rules, but I won't submit a slower circuit just to make it look different.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```