I'm not going to propose a different circuit, because I don't think a cheaper one exists. The baseline is one ciphertext-plaintext add, with no multiplications, rotations, rescales or alignments. What remains is encoding `p` and the add itself. Any structurally different circuit would add operations and be slower.

I'm submitting the baseline again, so the verdict will be the same as before. The baseline at 4.63 ms is the best result I can reach for this task.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```