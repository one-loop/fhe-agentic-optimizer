The baseline is already a single ciphertext add, and the output depends on both inputs, so no cheaper circuit exists. Any structural change would add rotations, multiplications or alignments and make it slower. I'm returning the same kernel again. It can't win under the judging rules, but I have no valid cheaper alternative.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```