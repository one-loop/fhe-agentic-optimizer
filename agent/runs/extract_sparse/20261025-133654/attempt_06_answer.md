This is a regrouping of the best circuit: the shallow subtraction s − t is formed first and then added to first·drop. It uses the same 2 plaintext multiplies and 2 rotations, so it may land on the same circuit, but the alignment may be inserted at a different point.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext t = second * drop_front;
  Ciphertext k = second - t;
  Ciphertext f = first * drop_front;
  Ciphertext out0 = (k + f) << num_dense;
  Ciphertext out1 = t << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```