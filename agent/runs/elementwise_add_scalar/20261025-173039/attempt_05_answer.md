The circuit is already a single ciphertext-plaintext add, with no multiplications, rotations, rescales or alignments. I can't find a cheaper valid circuit, and I have no way to remove that add while still using `s`. Any alternative I can think of keeps the add and adds operations, or breaks the CKKS rules.

I'm submitting the same minimal kernel once more. It matches the baseline circuit, so it can't become a new best. It stays correct and as fast as the baseline.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```