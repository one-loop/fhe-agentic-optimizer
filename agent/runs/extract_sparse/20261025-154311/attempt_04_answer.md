I don't have a circuit that beats the current best. Every variant I can build either keeps one level alignment (`match_scale`) or costs an extra plaintext multiplication or rotation. The reasons:

- **Combine before rotating:** this is the largest saving, and attempt 1 already has it.
- **Fewer than two rotations:** `output_0` is full-width and `output_1` fills 3/4 of the slots, so the two outputs can't share a ciphertext. There is also no rotated copy of the masks to apply after rotating.
- **No alignment, no extra multiply:** `second - t` needs `second` aligned to the masked level. Recomputing `second*drop` instead costs a 7 ms plaintext multiplication against a ~5 ms alignment, which is what attempt 2 measured.

I'm resubmitting the best kernel unchanged.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext a = first * drop_front;
  Ciphertext t = second * keep_front;
  Ciphertext out0 = (a + t) << num_dense;
  Ciphertext out1 = (second - t) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```