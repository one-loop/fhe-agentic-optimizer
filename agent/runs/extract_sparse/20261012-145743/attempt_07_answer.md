I'm submitting the attempt-1 kernel again. Attempt 6 measured 63.59 ms, the same as the others within noise, and no variant has beaten 63.53 ms. The kernel uses two ciphertext-plaintext multiplies and two rotations, which is the minimum I found for this formulation. The one scale alignment that remains costs about as much as any way I found to remove it.

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