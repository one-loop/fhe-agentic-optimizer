I don't have a cheaper circuit to offer. `x0 * s` is already the minimum: one plaintext multiply and one rescale, with no ciphertext multiplies, rotations, or alignments. Any correct rewrite I can construct only adds operations. For example, `(x0+x0)*s - x0*s` gives the right result but needs two plaintext multiplies. The commuted form was already reported as the same circuit as the baseline, so reordering the operands can't help either.

I'm resubmitting the baseline kernel as my best answer. It will be reported as identical to the baseline again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```