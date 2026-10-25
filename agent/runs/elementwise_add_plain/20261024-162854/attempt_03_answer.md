I still can't find a circuit cheaper than the baseline. The output needs `x0 + p` exactly, so the kernel needs one add, and `p` must be encoded for it. Every change I can think of keeps that add and puts extra rotations, multiplications, rescales or alignments on top of it. Any rewrite that leaves the circuit unchanged is judged identical to the baseline and cannot win.

I'm returning the baseline kernel again. It is already at the minimum operation count, and I'd rather not submit a slower circuit just to make it look different.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```