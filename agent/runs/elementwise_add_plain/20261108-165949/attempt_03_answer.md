The baseline is already the minimal circuit: one plaintext add, with no multiplications, rotations, rescales or levels. I can't find a structurally different circuit that is cheaper, and a different circuit would only add operations. Resubmitting the same kernel can't win, so I'm stopping here and keeping the baseline as the best.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```