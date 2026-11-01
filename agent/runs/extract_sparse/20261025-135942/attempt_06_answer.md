I could not find a circuit cheaper than attempt 1. Both rotations and both plaintext multiplies look necessary. The outputs together hold more data than one ciphertext, so they cannot share a rotation. Each output also needs a mask that `keep_front` or `drop_front` provides only via a multiply. Attempts 2–5 were the same circuit or slower, so I am resubmitting the best kernel unchanged:

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