The baseline is already at the minimum for this task: one add, with no multiplications, rotations, rescales or levels. Reordering the operands (attempt 2) produced the same circuit, and any structurally different circuit would need extra operations and run slower.

I have no kernel that is both different and cheaper, so I'm keeping the baseline form. Spending the remaining attempts on slower variants would not help.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```