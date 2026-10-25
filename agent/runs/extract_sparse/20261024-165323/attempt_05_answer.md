Idea: use `keep` instead of `drop` for the shared product. Let `k = second*keep`. Then `out0 = rot(first*drop + k)` and `out1 = rot(second - k)`, since `second*drop = second - second*keep`. This is 2 plaintext multiplies and 2 rotations, with the subtraction done before the rotation. The unmasked `second` is combined with `k` at a different level, which CHEHAB aligns itself.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext k = second * keep_front;
  Ciphertext t = first * drop_front;
  Ciphertext out0 = (t + k) << num_dense;
  Ciphertext out1 = (second - k) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```