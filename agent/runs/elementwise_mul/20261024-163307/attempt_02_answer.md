I don't have a cheaper circuit for this task. A product of two independent ciphertexts needs one ciphertext-ciphertext multiplication, with its relinearization and rescale. That is exactly what the baseline does.

The alternatives I considered are all worse:
- **Polarization identity**, `(x0+x1)² − x0² − x1² = 2·x0·x1`: this takes three squares, which is more work. It also needs a factor of 1/2, which CKKS can't write as a constant.
- **Plaintext multiplications or rotations**: neither helps, because both operands are ciphertexts.
- **Explicit rescale or mod-switch manipulation**: CHEHAB inserts these itself, and any extra operation only adds cost.

So I'm resubmitting the minimal kernel. It will again be judged the same circuit as the baseline and can't win on timing.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```