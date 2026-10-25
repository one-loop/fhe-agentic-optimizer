I don't have a further improvement that I can justify. The best kernel stays at 2 plaintext multiplies, 2 rotations and 1 `match_scale`. The measured times for it (130 to 139 ms) differ only by noise. I'm submitting the same kernel again.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext t = (first - second) * keep_front;
  Ciphertext mixed = first - t;
  Ciphertext out0 = mixed << num_dense;
  Ciphertext out1 = (second * drop_front) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```