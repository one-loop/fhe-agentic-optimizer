I'm submitting the same kernel again: no cheaper circuit exists for this task. It is a single add-plain with no multiplications, rotations, rescales or alignments, which is the least work that can compute `x0 + p`. Any structural change only adds operations, so I recommend stopping here.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```