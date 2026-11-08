The baseline circuit is already minimal, and I can't find a cheaper one. Computing `x0 * s` with a run-time scalar needs one ciphertext-plaintext multiplication and one rescale, with no relinearization, rotations or alignments. I won't resubmit the same kernel again, since that has repeated the baseline three times. I'm stopping here with the baseline as the best.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```