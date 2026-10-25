Idea: use keep = 1 - drop to write `first*drop + second*keep` as `(first - second)*drop + second`. This keeps 2 plaintext multiplies and 2 rotations. The subtraction now happens before the multiply, so the circuit is a different shape from attempt 2 (no extra subtraction at the aligned level). `second` is still added at the shallower level, so CHEHAB aligns it cheaply.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext s1 = second * drop_front;
  Ciphertext d = (first - second) * drop_front;
  Ciphertext comb = d + second;
  Ciphertext out0 = comb << num_dense;
  Ciphertext out1 = s1 << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```