I have no structurally cheaper circuit left to try. Attempts 3–6 all reduced to attempt 1's circuit, and the only distinct alternatives I can build are costlier:
- Masking both terms with `keep_front` needs two scale alignments.
- Aligning after the rotation needs three rotations.
- Using three multiplies instead of the alignment (attempt 2) was no faster.

Attempt 1 remains the best: 64.28 ms against 88.05 ms for the baseline. It uses 2 plaintext multiplies, 2 rotations and 1 `match_scale`. I'm resubmitting it below.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext t1 = second * drop_front;
  Ciphertext t0 = first * drop_front;
  Ciphertext out0 = ((t0 - t1) + second) << num_dense;
  Ciphertext out1 = t1 << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```