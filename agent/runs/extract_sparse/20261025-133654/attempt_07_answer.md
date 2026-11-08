I have no new circuit that beats the best. Two plaintext multiplies and two rotations look like the floor: the two outputs need two independent masked quantities, and a single rotation cannot serve both. I'm resubmitting the current best kernel (attempt 1).

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext t = second * drop_front;
  Ciphertext f = first * drop_front;
  Ciphertext out0 = (f - t + second) << num_dense;
  Ciphertext out1 = t << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```