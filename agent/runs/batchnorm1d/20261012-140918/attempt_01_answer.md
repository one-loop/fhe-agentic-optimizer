The baseline already uses the minimum possible work: one ciphertext-by-plaintext multiply (required, since A is per-slot and unknown until runtime) and one plaintext add (required for B). Rotations, ciphertext multiplies, and extra levels can't reduce this. Rewriting as `(x + B/A) * A` would need a division, which CKKS doesn't support, and any other reordering keeps the same operation count. I'm submitting the same kernel unchanged.

```cpp
// y = x * A + B: one plaintext multiply (depth 1) and one plaintext add.
// This is already the minimal operation set: no rotations, no ciphertext-ciphertext multiplies.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```