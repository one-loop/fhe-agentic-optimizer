I have no cheaper circuit to offer. The function needs both inputs combined, which takes one ciphertext add, and the baseline already does exactly that with no rotations, multiplications, rescales or alignments. Any circuit that differs from it would have to add operations and would be slower. Resubmitting the same kernel again cannot change the verdict, so this is the final form of the kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```