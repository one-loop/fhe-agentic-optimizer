The baseline is already a single ciphertext addition, which is the minimum for this function. Every operation count other than add is zero, so no multiplications, rotations, rescales or alignments remain to remove. Any change would either leave the circuit identical, which cannot win, or add work. I'm resubmitting the same kernel as the best available answer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```