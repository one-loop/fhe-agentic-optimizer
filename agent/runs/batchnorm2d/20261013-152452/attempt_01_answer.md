The baseline already uses the minimum work for this function: one ciphertext-plaintext multiply by `A` (one level, one rescale) and one plaintext add of `B`. There's no rotation, no ciphertext-ciphertext multiply, and no alignment step. Any rewrite would need to remove the multiply or the add, and neither can be removed without changing the result, so I'm returning the same kernel.

```cpp
// y = x * A + B. This is already the minimal operation count:
// one ct-pt multiply (one level) and one plaintext add.
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```