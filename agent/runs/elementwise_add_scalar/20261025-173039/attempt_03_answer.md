The circuit is already one ciphertext-plaintext add, with no multiplications, rotations, rescales or alignments. I can't find a legitimately cheaper circuit. Any other form needs the same add plus extra operations, or is rejected by the CKKS rules (no constants, no plaintext-plaintext arithmetic).

I'm resubmitting the minimal kernel. It will match the baseline circuit, so it won't count as a new best, but anything else I could write would be slower.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```