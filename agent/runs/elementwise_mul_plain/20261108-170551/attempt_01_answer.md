The baseline is already one plaintext multiply plus one rescale. That is the minimum for a depth-1 product with a plaintext vector, so I see no circuit with fewer multiplications, rotations, rescales or levels. Any rewrite would only add operations such as adds or alignments. I'm resubmitting the minimal form, and it probably won't count as a new best because its circuit matches the baseline's.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```