The baseline is already one ciphertext add, which is the minimum for a function of two inputs. Every circuit I can build that differs from it adds operations (rotations, multiplications, rescales or alignments) and would be slower. I can't produce a cheaper circuit, so I'm resubmitting the same kernel. It will again be judged identical to the baseline.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```