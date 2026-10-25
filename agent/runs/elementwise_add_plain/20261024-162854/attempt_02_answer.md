I can't find a cheaper circuit for this task. The kernel is one ciphertext-plaintext add, with no multiplications, rotations, rescales or alignments. The remaining cost is encoding `p` and the add itself.

Every alternative I can think of adds operations:
- Negating `p` is plaintext arithmetic, which is not allowed.
- Rotating, multiplying or mod-switching `x0` first costs more than the small encoding saving at a lower level.

I'm resubmitting the minimal form. It will probably be rejected as identical to the baseline, which is the best achievable here.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```