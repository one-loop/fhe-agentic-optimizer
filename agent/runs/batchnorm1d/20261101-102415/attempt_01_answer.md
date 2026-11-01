The baseline is already minimal, so I am returning it unchanged; it cannot win under the rules.

`y = x * A + B` takes one ciphertext-plaintext multiply, its rescale, and one plaintext add. These are all unavoidable:

- **The multiply.** The output depends multiplicatively on the per-slot values in `A`, and those only arrive as a plaintext.
- **The add.** `B` has to enter the result somehow.
- **The rescale.** CHEHAB inserts it automatically after the multiply.

The one known trick is to factor into `(x + B/A) * A`, which would turn the add into a cheaper encode at a deeper level. It needs `B/A`, which is plaintext-plaintext arithmetic and is not allowed. No numeric constants are allowed either.

Any circuit that differs from the baseline would have to add operations, such as rotations, extra multiplies or alignments. It would therefore be slower, not 3% faster.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```