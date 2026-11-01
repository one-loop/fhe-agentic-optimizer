The remaining cost is the scale alignment (`match_scale`) on the un-multiplied `second`. I'll try a different circuit: compute A = second·drop and B = first·drop, then out0 = (B − A + second) << n and out1 = A << n. Subtracting A from `second` first (at the product level) may let CHEHAB align the terms differently. It still uses 2 plaintext multiplications and 2 rotations.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext a = second * drop_front;
  Ciphertext b = first * drop_front;
  Ciphertext out0 = (b + (second - a)) << num_dense;
  Ciphertext out1 = a << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```