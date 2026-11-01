I have no cheaper structure left, so I'm submitting attempt 1 again for the last slot. It remains the best result: 64.28 ms against 88.05 ms for the baseline, about 1.38× faster.

The circuit seems to be at its floor:
- **Rotations (2):** each output needs one, since the two hold unrelated data.
- **Plaintext multiplies (2):** `first` and `second` each need one mask, and the masks cannot be rotated.
- **Alignment (1 `match_scale`):** it is cheaper than the third multiply that would remove it (attempt 2: 65.67 ms vs 64.28 ms).

Every other rewrite either reduced to this same circuit or added a rotation or a second alignment.

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