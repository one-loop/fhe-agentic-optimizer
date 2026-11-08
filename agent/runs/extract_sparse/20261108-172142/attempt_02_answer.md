`first*drop + second*keep` equals `first + (second - first)*keep`, which needs one plaintext multiplication instead of two. The kernel now has 2 plaintext multiplications and 2 rotations. The cost is one level alignment when `first` is added to the product.

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