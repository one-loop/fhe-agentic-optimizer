I have no cheaper circuit to offer. Four attempts have all produced the baseline's circuit: one plaintext multiply and one rescale on a single level. For `y = x0 * p` that is the minimum, with no rotations, ciphertext multiplications or alignments left to remove.

Any structurally different kernel would add operations and be slower, so it could not become the new best. I'm submitting the baseline again as the best kernel I know, and I'm leaving the remaining attempts unused.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```