This variant uses K = second·keep, which is shared between both outputs. out0 = (first·drop + K) << n, and out1 = (second − K) << n, since second·drop = second − second·keep. It has 2 plaintext multiplications and 2 rotations, but the circuit structure differs from the current best, and the alignment of `second` now sits on the out1 path.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext k = second * keep_front;
  Ciphertext out0 = (first * drop_front + k) << num_dense;
  Ciphertext out1 = (second - k) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```