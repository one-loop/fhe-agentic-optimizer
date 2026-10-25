The baseline is a single add, so any different circuit has to be an equivalent form of that add. The options I can see are:

- `s + x0`: CHEHAB likely normalizes this to the same circuit as the baseline.
- `x0 - (-s)`: plaintext negation is not allowed, since every operation needs a ciphertext operand.
- `x0 += s`: the same circuit as the baseline.
- `-(-x0 - s)`: this adds two negations, which is more work.

None of these can beat one add. The one structurally different option is to compute `(x0 - s)` first and then negate. That is not equivalent, so it is not an option either.

I'll try a variant that is a different circuit but costs about the same: `y = (x0 + s)` computed as `(x0 + x0) + s` minus `x0`. This adds operations and will probably be slower, but it does test whether the timing is noise-dominated. A cheaper circuit than one add does not exist, so I'm submitting the minimal form again, written with `-=` on a negated scalar to see whether CHEHAB normalizes it differently.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0;
  y += s;
  y.set_output("y");
}
```