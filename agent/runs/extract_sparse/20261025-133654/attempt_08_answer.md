This is my last attempt. It formulates the problem with the `keep_front` mask instead of `drop_front`, using m = (second − first)·keep and k = second·keep:

- out0 = (first + m) << n, which equals first on the dropped region and second on the kept region.
- out1 = (second − k) << n.

It still uses 2 plaintext multiplies and 2 rotations, but with two unmasked terms to align it will probably not beat the current best.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext m = (second - first) * keep_front;
  Ciphertext k = second * keep_front;
  Ciphertext out0 = (first + m) << num_dense;
  Ciphertext out1 = (second - k) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```