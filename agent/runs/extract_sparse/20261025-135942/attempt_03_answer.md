Idea: use `first*drop + second*keep = (first - second)*drop + second`. With `v = second*drop` for `out1`, this uses the same two plaintext multiplies, but the subtraction happens before the multiply. The counts should match attempt 1, so this is probably no faster, but it is a distinct circuit.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext d = first - second;
  Ciphertext p = d * drop_front;
  Ciphertext v = second * drop_front;
  Ciphertext out0 = (p + second) << num_dense;
  Ciphertext out1 = v << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```