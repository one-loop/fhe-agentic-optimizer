I'm stopping here: there is no cheaper circuit to try, and I won't spend the remaining attempts on variants that CHEHAB normalizes to a circuit already evaluated.

Computing x² takes exactly one ciphertext multiplication, with one relinearization and one rescale. That is the minimum for this task, and it is the circuit the baseline already uses. Every alternative I can build is one of two things:
- **Slower:** it adds multiplications, rotations, or plaintext operations.
- **Wrong:** it computes 2x² or 4x², and undoing that needs a constant, which CKKS doesn't allow here.

The best kernel remains the baseline, repeated below.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```