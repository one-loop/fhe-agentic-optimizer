Attempt 2 resubmitted the same kernel, so the 130 vs 139 ms gap is just timing noise. This time I changed the structure: out0 is now `first + (second - first) * keep_front`, which is the same value but uses an add in place of a subtract. The aim is to see whether CHEHAB handles the level alignment of `first` more cheaply. The op counts stay at 2 plaintext multiplies and 2 rotations.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext d = (second - first) * keep_front;
  Ciphertext mixed = d + first;
  Ciphertext out0 = mixed << num_dense;
  Ciphertext out1 = (second * drop_front) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```