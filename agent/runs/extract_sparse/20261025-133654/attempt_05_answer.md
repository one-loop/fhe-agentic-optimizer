This variant derives out1 after the rotations: out1 = rot(second) − rot(k), where k = second·keep. It costs one more rotation (3), but the unmasked `second` is rotated directly and is subtracted from a masked term, so CHEHAB may need only a cheap mod switch instead of a multiply-based alignment. It is a different circuit, and I'm testing whether it pays off.

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
  Ciphertext out0 = (f + k) << num_dense;
  Ciphertext out1 = (second << num_dense) - (k << num_dense);
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```