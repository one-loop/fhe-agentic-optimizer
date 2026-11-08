The rotation is linear, so the two rotated terms of `output_0` can be summed first and rotated once. That brings the kernel from 3 rotations to 2, with the same 3 plaintext multiplications.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext sum0 = (first * drop_front) + (second * keep_front);
  Ciphertext out0 = sum0 << num_dense;
  Ciphertext out1 = (second * drop_front) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```