The last attempt matched the baseline within noise (9.66 ms vs 9.59 ms), and the operation counts are identical. The kernel performs one plaintext multiply and one plaintext add, and I see no way to remove either. Dropping the add would change the function. Reordering or rewriting as `(x + B/A) * A` needs a division, which CKKS doesn't support. The remaining levers (rotations, ciphertext multiplies, depth) are not in use. I'm resubmitting the baseline kernel as the best version.

```cpp
// y = x * A + B: one plaintext multiply (depth 1) and one plaintext add.
// Already minimal for this function; no rotations or ciphertext multiplies.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```