This is the last attempt, and I have no cheaper circuit for it. `y = x²` already costs the minimum: one squaring, plus the relinearize and rescale that CHEHAB inserts. Every other correct circuit adds work.

Final outcome for the quad task:
- **Best result:** the baseline, at 6.06 ms.
- **Closest variant:** `square(x)`, about 2% faster (5.91 ms against 6.05 ms, faster in 4 of 5 batches). That is below the 3% threshold, so it was not confirmed.
- **Attempts 2–7:** resubmissions of the same circuit, which I should have stopped sooner. They tested nothing new.

The kernel below is attempt 1's, the best found, given because the format requires one.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```