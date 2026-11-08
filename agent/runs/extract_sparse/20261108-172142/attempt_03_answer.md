This variant reuses `second*drop`: `second*keep` becomes `second - second*drop`, which needs no multiplication. There are still 2 plaintext multiplications and 2 rotations, but the add structure differs, so I'll test whether it changes the alignment cost.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext w = second * drop_front;
  Ciphertext sum0 = ((first * drop_front) - w) + second;
  Ciphertext out0 = sum0 << num_dense;
  Ciphertext out1 = w << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```