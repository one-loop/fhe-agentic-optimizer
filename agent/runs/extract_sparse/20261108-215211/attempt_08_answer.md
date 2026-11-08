This is the last attempt, and I don't have a cheaper circuit to try, so I'm submitting attempt 2 again as the final kernel. It will be marked "same as best", but it is the fastest correct kernel found: 12.14 ms against the baseline's 14.16 ms, faster in 5 of 5 timed batches.

The circuits I could still build are all ones already timed or expected to be slower:
- **Two masks instead of three:** every way of writing this, with `keep_front` or `drop_front`, reproduces attempt 1's operation counts. That includes a scale alignment that costs as much as the mask it saves, and that circuit lost its timed comparison.
- **Three masks plus an alignment, or a third rotation:** these do strictly more work than attempt 2.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext shifted1 = (second * drop_front) << num_dense;
  Ciphertext merged = first * drop_front + second * keep_front;
  Ciphertext out0 = merged << num_dense;
  out0.set_output("output_0");
  shifted1.set_output("output_1");
}
```