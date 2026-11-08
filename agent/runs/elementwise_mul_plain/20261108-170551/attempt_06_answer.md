The baseline circuit is already minimal, and I have no cheaper alternative. All five attempts so far reduced to it: one plaintext multiply and one rescale on a single level. Any different circuit would add operations, such as extra plaintext multiplies, adds or alignments, so it would be slower and could not become the new best.

I'm submitting the baseline form again and leaving the remaining attempts unused.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```