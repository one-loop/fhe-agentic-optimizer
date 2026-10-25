I could not find a circuit with fewer operations than the current best (2 plaintext multiplies, 2 rotations, 1 alignment). Every alternative I tried either needs a third rotation (~20 ms) or a third plaintext multiply, which costs more than the ~5 ms alignment it would remove. I am returning the best kernel unchanged.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext k = second * keep_front;
  Ciphertext t = first * drop_front;
  Ciphertext out0 = (t + k) << num_dense;
  Ciphertext out1 = (second - k) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```