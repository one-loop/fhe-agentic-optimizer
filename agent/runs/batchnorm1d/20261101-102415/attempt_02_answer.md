No cheaper circuit exists for this task, so I recommend stopping here. The kernel below is the baseline again; it will score as identical and cannot win.

The baseline already has the minimum possible circuit for `y = x*A + B`:

- **The multiply.** A real per-slot scale can only enter through a ciphertext-plaintext multiply by `A`.
- **The rescale.** CHEHAB inserts it automatically after that multiply, so I cannot remove it.
- **The add.** One plaintext add is the only way to bring in `B`.

The alternatives I can see are all ruled out or slower:

- **Factoring into `(x + B/A) * A`.** This would need `B/A`, which is plaintext-plaintext arithmetic, and that is forbidden.
- **Synthesizing constants or using rotations.** These would need numeric constants, which are banned, or rotations, which are not needed.
- **Adding or splitting operations.** Splitting the multiply or adding extra adds would add operations and make it slower.

Every structurally different kernel would be slower, so spending more attempts would only produce worse timings.

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