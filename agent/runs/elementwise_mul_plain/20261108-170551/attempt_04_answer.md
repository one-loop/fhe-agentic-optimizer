I have no cheaper circuit to propose, and I'll stop submitting variants. Three attempts have all reduced to the baseline's circuit: one plaintext multiply and one rescale on a single level. That is already the minimum for this function.

Any structurally different kernel would add operations, such as extra plaintext multiplies, adds or level alignments. It would be slower and could not become the new best. The baseline is the best kernel I can offer, and I'm leaving the remaining attempts unused.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```