This variant uses the identity `out0 = ((first*drop + second) << n) - out1`. The rotated sum is shared with the second output, so each output still needs one rotation and one mask multiply. I don't see a way to remove either: both outputs need their own rotation and their own mask.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext a = first * drop_front;
  Ciphertext b = second * drop_front;
  Ciphertext out1 = b << num_dense;
  Ciphertext e = a + second;
  Ciphertext out0 = (e << num_dense) - out1;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```