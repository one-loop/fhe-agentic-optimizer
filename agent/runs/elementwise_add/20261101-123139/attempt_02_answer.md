There is no cheaper circuit than this one. The output needs information from both ciphertexts, which takes at least one ciphertext operation. The current circuit uses exactly one addition and nothing else, so no other circuit can have a lower operation count. Any circuit that differs from it must add work (adds, rotations, multiplications or alignments) and would be slower.

I recommend stopping here: the baseline is optimal for this task. The kernel below is the baseline again, unchanged. It will not register as a new best, and I would not use more attempts on rewrites that are bound to be slower.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```