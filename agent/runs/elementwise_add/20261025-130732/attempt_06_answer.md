The baseline is already one ciphertext add, and the output depends on both inputs, so at least one operation is required. Any structurally different circuit would add rotations, multiplications or alignments and cost more. I have no cheaper circuit, so I'm returning the same kernel. It can't win under the judging rules, but it is the minimum.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```