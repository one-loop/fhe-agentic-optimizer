I'm submitting the baseline again because no cheaper circuit exists. It will register as identical and cannot win.

The kernel already uses only what `y = x*A + B` needs: one multiply by plaintext `A`, the one rescale CHEHAB inserts after it, and one plaintext add of `B`. There are no rotations, ciphertext-ciphertext multiplies or alignments to remove. The only factoring that could save work, `(x + B/A)*A`, needs plaintext-plaintext arithmetic, which is forbidden. Any other structure adds operations and runs slower, so the remaining attempts can't improve on 9.86 ms. I'd recommend ending the search here.

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