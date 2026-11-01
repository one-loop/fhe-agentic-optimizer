I'm returning the attempt-1 kernel as the final answer. It is the fastest measured kernel (63.53 ms, against 87.72 ms for the baseline), it is correct on every case, and the repeated runs of this structure (63.55–63.60 ms) are within noise of it.

The idea: since `keep + drop = 1`, `second*keep = second - second*drop`. Computing `t = second*drop` once lets `output_1 = t << n` and `output_0 = (first*drop + second - t) << n` share it. That gives two ciphertext-plaintext multiplies and two rotations, instead of the baseline's three and three.

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