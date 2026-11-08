This is the last attempt, and the conclusion hasn't changed: `y = x0 + x1` needs exactly one ciphertext add, and CHEHAB reports it as the only operation (add=1, everything else 0). A circuit with fewer operations doesn't exist, and anything structurally different would add rotations, multiplications or alignments and so be slower. I'm submitting the same minimal kernel. It will match the baseline circuit, so it can't be ranked faster, and the baseline stays the best.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```