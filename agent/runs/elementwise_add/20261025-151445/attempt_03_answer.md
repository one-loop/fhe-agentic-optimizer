The kernel is already one ciphertext add, which is the minimum for combining two encrypted inputs. There are no multiplications, rotations, rescales or alignments left to remove. Every alternative I can build either normalizes to the same circuit, so it cannot win, or adds operations and runs slower. I'm resubmitting the same kernel, and it will likely be judged identical to the baseline again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```