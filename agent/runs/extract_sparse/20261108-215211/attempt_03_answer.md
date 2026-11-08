Attempt 2 is the best so far: it shares one rotation for `output_0` and keeps every term at the same level.

This attempt retries a two-multiply version with a different arrangement:
- **Merged term.** `first*drop + second*keep` is computed as `(first - second) * drop_front + second`. The subtraction happens at the top level before masking, where a ciphertext add is cheap.
- **`output_1`.** It still uses its own masked product `second * drop_front`.

The circuit has 2 plaintext multiplies and 2 rotations, plus one alignment of the unmasked `second`. That alignment is the cost that made attempt 1 lose its timed comparison. This structure differs from attempt 1, so the trade may or may not come out ahead.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext shifted1 = (second * drop_front) << num_dense;
  Ciphertext merged = (first - second) * drop_front + second;
  Ciphertext out0 = merged << num_dense;
  out0.set_output("output_0");
  shifted1.set_output("output_1");
}
```