I don't have a cheaper circuit. Seven attempts have all reduced to the baseline's: one plaintext multiply and one rescale on a single level, with no rotations, ciphertext multiplications or alignments. That is the minimum for `y = x0 * p`. Any structurally different kernel would add operations and be slower, so it could not become the best.

The baseline remains the best kernel. I'm submitting it unchanged for the last attempt.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```