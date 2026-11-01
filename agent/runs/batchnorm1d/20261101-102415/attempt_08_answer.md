For this last attempt I am resubmitting the baseline unchanged; I could not find a faster kernel. The 9.86 ms baseline stands as the final result.

The baseline already has the fewest operations `y = x*A + B` allows: one plaintext multiply by `A`, the one rescale CHEHAB inserts after it, and one plaintext add of `B`. Removing the multiply or the rescale would mean factoring it as `(x + B/A)*A`. That needs plaintext-plaintext arithmetic, which is not allowed. Any other structure adds rotations, multiplies or alignments, so it would only be slower.

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