The baseline is already a single ciphertext add, which is the minimum for this function. The output depends on both inputs, so at least one operation is needed. Every structurally different circuit I can think of adds rotations, multiplications or alignments, which would cost more. I'm submitting the same kernel one last time. It cannot beat the baseline under the judging rules, and I have no cheaper valid circuit to offer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```