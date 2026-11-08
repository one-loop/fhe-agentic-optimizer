I haven't found a cheaper circuit than attempt 2's. A single plaintext multiplication would need both masked quantities packed into one ciphertext, and that takes an extra rotation (~20 ms) to save a ~7 ms multiplication. I'm resubmitting the best kernel unchanged.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext sum0 = first + ((second - first) * keep_front);
  Ciphertext out0 = sum0 << num_dense;
  Ciphertext out1 = (second * drop_front) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```