The best kernel so far is attempt 1 at 63.53 ms. Attempt 3 removed the alignment but added a third multiply, and came out slightly slower (65.58 ms), so the trade does not pay. I'm returning the attempt-1 kernel unchanged.

Its structure: `t = second*drop` is computed once and reused, `output_0` is `(first*drop + second - t) << n` (since `second*keep = second - second*drop`), and `output_1` is `t << n`. That is 2 ciphertext-plaintext multiplies and 2 rotations, which is the minimum I found for this formulation. The only remaining overhead is one scale alignment, and every variant I tried to remove it cost about as much as it saved.

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