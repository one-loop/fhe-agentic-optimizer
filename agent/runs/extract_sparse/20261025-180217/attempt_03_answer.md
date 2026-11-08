The previous attempt compiled to the same circuit as attempt 1, so this one changes the structure. It uses the keep mask directly instead of `second - second*drop`. That removes the level/scale alignment of `second`, at the cost of a third plaintext multiply. It also merges the first two rotations into one, so the circuit is 3 plaintext multiplies and 2 rotations.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext a = first * drop_front;
  Ciphertext c = second * keep_front;
  Ciphertext b = second * drop_front;
  Ciphertext out0 = (a + c) << num_dense;
  Ciphertext out1 = b << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```