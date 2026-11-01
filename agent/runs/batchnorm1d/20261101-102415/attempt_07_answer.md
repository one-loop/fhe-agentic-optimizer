I am submitting the baseline again (attempt 7). It cannot win, because no cheaper circuit exists for this task.

`y = x*A + B` needs at least one multiply by `A` (with the rescale CHEHAB adds) and one add of `B`, which is exactly what the baseline does. The one rewrite that could save work, `(x + B/A)*A`, needs plaintext-plaintext arithmetic, which isn't allowed. Any other structure adds operations and runs slower. The 9.86 ms baseline is the final result.

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