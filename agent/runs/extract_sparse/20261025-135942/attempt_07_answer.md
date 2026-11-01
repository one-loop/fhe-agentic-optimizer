I found no circuit cheaper than attempt 1. Its 2 plaintext multiplies and 2 rotations look like the minimum for this task. I am resubmitting it unchanged.

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