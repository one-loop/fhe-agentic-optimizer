The baseline is already a single ciphertext add, which is the minimum for a function of two inputs. Any structurally different circuit would add rotations, multiplications, rescales or alignments, and would be slower. I have no cheaper circuit, so I'm submitting the same kernel again. It will be judged identical to the baseline.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```