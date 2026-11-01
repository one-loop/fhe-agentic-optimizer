The negation variant added two operations and measured 3.10 ms against 2.70 ms, as expected. The baseline's single plaintext add is still the minimum circuit, with no multiplications, rotations, rescales or alignments to remove. Every other formulation I can build either adds operations or normalizes back to it, so I'm submitting the baseline form again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```