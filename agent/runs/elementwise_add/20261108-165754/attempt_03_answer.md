The kernel is still a single ciphertext add, which is the minimum for x0 + x1. It has no multiplications, rotations, rescales or levels to remove. Any structurally different circuit would have to add operations, so it could only be slower. I'm submitting the same kernel again, and it will again match the baseline circuit and can't be ranked faster.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```