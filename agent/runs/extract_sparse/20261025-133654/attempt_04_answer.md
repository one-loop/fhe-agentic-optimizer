This variant computes the kept part first, k = second·keep, and then derives the dropped part by subtraction, t = second − k. The outputs are out0 = (first·drop + k) << n and out1 = t << n. It keeps 2 plaintext multiplies and 2 rotations, but the unmasked `second` term is aligned in a different place, so it may land on a cheaper schedule.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext f = first * drop_front;
  Ciphertext k = second * keep_front;
  Ciphertext t = second - k;
  Ciphertext out0 = (f + k) << num_dense;
  Ciphertext out1 = t << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```