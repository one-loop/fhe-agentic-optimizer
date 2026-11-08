I still have no cheaper circuit. The baseline is one ciphertext + plaintext add with no multiplications, rotations, rescales or alignments, so there is nothing left to remove. Any variant I could write either normalizes to the same circuit, which gets rejected as identical, or adds operations that cost more. I'm submitting the same minimal kernel again, and I expect the same verdict.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```