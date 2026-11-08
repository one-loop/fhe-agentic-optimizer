The baseline is already one ciphertext add. The output depends on both inputs, so at least one operation is needed, and every alternative I can think of only adds rotations, multiplications or alignments. I have no cheaper circuit to propose, so I'm returning the same kernel. It can't win under the judging rules, but changing it would make it slower.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```