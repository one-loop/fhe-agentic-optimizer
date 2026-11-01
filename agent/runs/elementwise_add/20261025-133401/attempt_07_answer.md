The kernel is still a single ciphertext add, which is the minimum for x0 + x1. Every circuit with a different operation count would add rotations, multiplications or alignments, and so be slower. I have no valid cheaper circuit, so I'm submitting the same kernel again. It will be judged identical to the baseline and cannot become the new best.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```