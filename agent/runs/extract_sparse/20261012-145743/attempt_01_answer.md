The baseline computes `((first*drop) + (second*keep)) << n` for `output_0` and `(second*drop) << n` for `output_1`. Since `keep + drop = 1`, `second*keep = second - second*drop`. So I compute `t = second*drop` once and reuse it, then form `output_0` as `(first*drop + second - t) << n`. This uses 2 ciphertext-plaintext multiplies and 2 rotations instead of 3 and 3. Both multiplies stay at depth 1.

```cpp
// Reuse t = second*drop; second*keep = second - t, so only 2 pt-mults and 2 rotations are needed.

void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext t = second * drop_front;
  Ciphertext a = first * drop_front;
  Ciphertext u = a + second - t;
  Ciphertext out0 = u << num_dense;
  Ciphertext out1 = t << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```