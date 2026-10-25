Attempts 1 and 2 used the same kernel, so the 133 ms vs 142 ms difference is measurement noise. This time I'll try a real change: compute `u = second * drop_front` directly. That replaces the `match_scale` alignment on `second` with a plaintext multiply, and the multiply keeps `u` at the same level and scale as `a` and `t`. It costs about the same as the alignment, but it removes the dependency of `u` on `t`, so the work is independent and the timing may be better.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext a = first * drop_front;
  Ciphertext t = second * keep_front;
  Ciphertext u = second * drop_front;
  Ciphertext out0 = (a + t) << num_dense;
  Ciphertext out1 = u << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```