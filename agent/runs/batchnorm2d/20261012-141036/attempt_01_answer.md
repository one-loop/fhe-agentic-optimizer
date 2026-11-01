The baseline already uses the minimum work for this function. `A` varies per slot, so one ciphertext-plaintext multiply (depth 1) is unavoidable. `B` is a per-slot plaintext shift, so one plaintext add is also required. No rotations or ciphertext-ciphertext multiplies are needed, and CKKS rules forbid constant tricks that could fold the shift into the multiply. I'm returning the same kernel unchanged.

```cpp
// y = x * A + B: one ciphertext-plaintext multiply (depth 1) plus one plaintext add.
// This is the minimal evaluation: no rotations, no ciphertext-ciphertext multiplies.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```