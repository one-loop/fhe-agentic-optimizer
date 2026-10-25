I did not find a cheaper circuit. This is the best kernel so far (attempt 5), unchanged:

- Both outputs need a mask on `second` and `out0` needs one on `first`, so 2 plaintext multiplies is the minimum.
- Both outputs need their own rotation, so 2 rotations is the minimum without rotated masks.
- The one remaining alignment (`second` meeting `k` before the subtraction) costs about 5 ms. Removing it needs a third rotation, which is more expensive (~20 ms).

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