I have no cheaper circuit to offer. The remaining alternatives either add a rotation (about 20 ms) or a plaintext multiply (about 7 ms, more than the roughly 5 ms alignment they would remove). For the final attempt I'm submitting the best kernel unchanged. It is correct and 1.38× faster than the baseline (63.5 ms vs 87.9 ms).

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext u = first * drop_front;
  Ciphertext t = second * keep_front;
  Ciphertext out0 = (u + t) << num_dense;
  Ciphertext out1 = (second - t) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```