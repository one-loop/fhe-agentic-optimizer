This is my last attempt and I haven't found a cheaper circuit than attempt 2, so I'm submitting that kernel again. Its cost is 2 plaintext multiplications, 2 rotations and 1 scale alignment, and it measured 63.5 ms against 87.8 ms for the baseline.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext diff = first - second;
  Ciphertext mixed = second + diff * drop_front;
  Ciphertext out0 = mixed << num_dense;
  Ciphertext out1 = (second * drop_front) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```