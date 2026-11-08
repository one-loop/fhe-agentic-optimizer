Both of my earlier forms use two plaintext multiplies and one scale alignment, because the unmasked `second` term has to be aligned to the multiplied level. This attempt masks `second` with `keep_front` as well, so every term of the sum sits at the same level and the alignment disappears. The cost is one extra plaintext multiply, which may or may not be cheaper than the alignment it replaces.

The structure is out0 = (first·drop + second·keep) << n and out1 = (second·drop) << n. That gives 3 plaintext multiplies, 2 rotations and no alignment.

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
  Ciphertext t = second * drop_front;
  Ciphertext out0 = (f + k) << num_dense;
  Ciphertext out1 = t << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```