I don't have a cheaper circuit to offer. `x0 * x1` is already one ciphertext-ciphertext multiply, one relinearization and one rescale, which is the minimum for a product of two ciphertexts.

Every algebraic alternative I can think of is worse or not allowed:
- **Squares:** the identities `(a+b)² − (a−b)² = 4ab` and `(a+b)² − a² − b² = 2ab` need two or three squares, and each costs as much as a multiply.
- **Scaling back:** those identities give 4ab or 2ab, so I'd need a 1/4 or 1/2 factor. Constants can only come from plaintext inputs, and this task has none.
- **Plaintext multiplies:** the task has no plaintext inputs, so they are not an option.
- **Rotations:** they do nothing for an elementwise product.
- **Relinearization and rescale:** CHEHAB inserts both automatically, so I can't remove or reorder them from the kernel.

Any different circuit would add operations and run slower than the baseline. I'm submitting the minimal kernel again. It will not become a new best, but I'd rather not trade correctness or speed for a different circuit.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```