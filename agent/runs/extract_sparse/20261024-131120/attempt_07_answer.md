My best result is still attempt 1 at 130.16 ms, down from 194.16 ms at baseline. Attempts 5 and 6 landed at 130.80 and 130.35 ms with the same kernel, so it is stable. I have no new idea that reduces the multiply or rotation count, so I'm submitting the same kernel again.

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