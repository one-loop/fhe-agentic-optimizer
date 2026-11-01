Idea: remove the scale alignment entirely. Every term is masked, so all operands sit at the same level. That is 3 plaintext multiplies but only 2 rotations and no `match_scale`. It is probably only about as fast as the best, but this checks whether the alignment costs more than a plain multiply.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext out0 = (first * drop_front + second * keep_front) << num_dense;
  Ciphertext out1 = (second * drop_front) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```