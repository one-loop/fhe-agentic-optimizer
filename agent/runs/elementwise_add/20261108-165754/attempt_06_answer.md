Still the same situation: `y = x0 + x1` is one ciphertext add, and no circuit with fewer operations can compute it. Any structural change would add operations (rotations, multiplications, alignments) and be slower. I'm submitting the same kernel again, and it will match the baseline circuit, so it can't be ranked faster.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```