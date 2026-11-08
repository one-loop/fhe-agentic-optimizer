This attempt keeps 2 plaintext multiplies and 2 rotations, but reorganizes the work around the keep mask. It builds `first + keep*(second - first)` before the rotation and uses `second*drop` for the second output. CHEHAB may compile it to the same circuit as the best one, but the dataflow is different.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext z = second - first;
  Ciphertext m = z * keep_front;
  Ciphertext pre0 = first + m;
  Ciphertext b = second * drop_front;
  Ciphertext out0 = pre0 << num_dense;
  Ciphertext out1 = b << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```