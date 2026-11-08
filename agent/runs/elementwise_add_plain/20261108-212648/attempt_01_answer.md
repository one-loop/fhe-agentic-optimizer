The baseline is already optimal, so I am returning it unchanged.

It computes `y = x0 + p` with one plaintext addition. There are no multiplications, rotations, rescales or alignments, and it consumes 0 levels. Every other circuit for this function adds at least one operation, so it would be slower. This kernel cannot become the new best, because it matches the baseline's circuit and the judge requires a different one. Any kernel that could pass that rule would run slower.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```