Attempt 2 is effectively identical to attempt 1 and shows the same plateau. The only remaining overhead is the `match_scale` from adding the level-0 `second` into level-1 terms. I'll try removing it by computing `second*keep` as its own multiply, so `out0` is `(first*drop + second*keep) << n` with both addends at the same level. This adds one ciphertext-plaintext multiply but removes the alignment, so the net gain may be small. The rotation count stays at 2.

```cpp
// out0 = (first*drop + second*keep) << n, out1 = (second*drop) << n.
// Both addends of the sum come from one multiply, so no scale alignment is needed.

void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext a = first * drop_front;
  Ciphertext k = second * keep_front;
  Ciphertext t = second * drop_front;
  Ciphertext out0 = (a + k) << num_dense;
  Ciphertext out1 = t << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```