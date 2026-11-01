I'm resubmitting the baseline because it is already the cheapest possible circuit for `y = x0 * p`.

The plaintext vector `p` is only known at run time and CKKS allows no constants, so the result needs exactly one ct-pt multiply. CHEHAB then adds its automatic rescale. Nothing is left to remove:
- no ct-ct multiplies,
- no rotations,
- no adds,
- no scale or level alignments.

Any different circuit would be the same one plus extra operations, so it would be slower. Further attempts on this task cannot yield a correct, faster kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```