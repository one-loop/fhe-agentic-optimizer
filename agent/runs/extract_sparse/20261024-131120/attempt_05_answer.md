The 3-multiply variant was slower (144.6 ms), so I'm going back to the best form: 2 plaintext multiplies and 2 rotations. I don't see a way to cut either count with the masks provided. `output_1` needs its own multiply and rotation, and `output_0` needs one shared rotation. The remaining differences between the 2-multiply attempts (130 to 139 ms) look like timing noise. I'm resubmitting the attempt 1 kernel.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext t = (first - second) * keep_front;
  Ciphertext mixed = first - t;
  Ciphertext out0 = mixed << num_dense;
  Ciphertext out1 = (second * drop_front) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```