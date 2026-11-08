This last attempt is a different formulation, though I expect it to be about the same speed or slightly slower. `output_0` uses `first + (second - first)*keep` and `output_1` uses `second - second*keep`. That is still 2 plaintext multiplications and 2 rotations, but it likely needs two level alignments instead of one. If it isn't faster, the attempt 1 kernel stays the best.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext d = second - first;
  Ciphertext m = d * keep_front;
  Ciphertext t = second * keep_front;
  Ciphertext out0 = (first + m) << num_dense;
  Ciphertext out1 = (second - t) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```