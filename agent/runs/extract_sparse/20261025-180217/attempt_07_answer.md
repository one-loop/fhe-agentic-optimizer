I couldn't find a cheaper structure. Each output needs its own rotation and its own mask multiply, and the one level alignment is unavoidable when `second` is mixed with masked terms. As a last exploration, this variant masks only `first` and `second*keep` (2 multiplies) and derives `out1` by subtracting the rotated kept part from the rotated raw `second`. It trades an extra rotation for different dataflow. I expect it to be slower than the best.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext a = first * drop_front;
  Ciphertext c = second * keep_front;
  Ciphertext rc = c << num_dense;
  Ciphertext out0 = (a << num_dense) + rc;
  Ciphertext out1 = (second << num_dense) - rc;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```