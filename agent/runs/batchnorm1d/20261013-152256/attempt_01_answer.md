The baseline already uses the minimum possible work: one ciphertext-plaintext multiply for `x * A` and one plaintext add for `+ B`. A and B are per-slot vectors, so they can't be folded into scalars, and there is no ciphertext-ciphertext multiply or rotation to remove. I don't see a rewrite that lowers the multiply count or the encoding cost, so I'm returning the same computation, written as two explicit steps.

```cpp
// y = x * A + B, the minimal evaluation: one ciphertext-plaintext multiply
// (one level) and one plaintext-vector add. No rotations or ciphertext
// multiplications are needed, so this is already the cheapest form.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext y = t + b;
  y.set_output("y");
}
```