I haven't found a circuit cheaper than attempt 2's. Going below 2 plaintext multiplications and 2 rotations needs either a third rotation or an extra multiplication, and both cost more than the single level alignment I'm trying to avoid. I'm resubmitting the best kernel unchanged. It is the same circuit, so it can't count as a new best.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext sum0 = first + ((second - first) * keep_front);
  Ciphertext out0 = sum0 << num_dense;
  Ciphertext out1 = (second * drop_front) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```