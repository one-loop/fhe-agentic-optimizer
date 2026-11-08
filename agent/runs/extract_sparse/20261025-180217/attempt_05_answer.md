This is the other way to reach `second*drop`: write it as `second - second*keep`. It keeps the 2-multiply, 2-rotation structure but with different dataflow, so it likely compiles to the same circuit as the best one. I don't see a cheaper algebraic form: each output needs its own rotation and its own mask multiply.

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
  Ciphertext b = second - c;
  Ciphertext out0 = (a + c) << num_dense;
  Ciphertext out1 = b << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```