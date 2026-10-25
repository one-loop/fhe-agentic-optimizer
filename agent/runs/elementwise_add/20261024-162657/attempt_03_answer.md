A single ciphertext add is the minimum for this function. The output depends on both inputs, so at least one binary operation is unavoidable. Any circuit that differs from the baseline would have to add multiplications, rotations, rescales or alignments, and would be slower. There is no cheaper circuit to propose, so I'm resubmitting the same kernel. It will be judged identical to the baseline and cannot win.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```