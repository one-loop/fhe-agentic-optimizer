Idea: `out0 = (first + (second - first)*keep) << d` and `out1 = (second*drop) << d`. This is still 2 plaintext multiplies, 2 rotations and one alignment (of `first`). It restructures where the subtraction and alignment happen, so it may shift the add and alignment costs slightly.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext a = (second - first) * keep_front;
  Ciphertext out0 = (first + a) << num_dense;
  Ciphertext out1 = (second * drop_front) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```