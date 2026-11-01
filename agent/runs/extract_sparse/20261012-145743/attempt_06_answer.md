The attempt-1 kernel stays the best at 63.53 ms, and this run (63.56 ms) is the same kernel within measurement noise. Its cost is two ciphertext-plaintext multiplies, two rotations, and one scale alignment, and I have not found a formulation that removes any of these without adding an equal or larger cost. I'm submitting it again unchanged.

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