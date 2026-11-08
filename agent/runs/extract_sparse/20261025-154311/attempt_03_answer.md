The idea is to cut one plaintext multiplication by folding the two masked terms of `output_0` into one. Since drop = 1 - keep, first*drop + second*keep = first + (second - first)*keep. That leaves 2 plaintext multiplications and 2 rotations. The alignment of `first` against the masked term may be a cheap mod switch instead of a multiply_plain plus rescale.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext d = second - first;
  Ciphertext t = d * keep_front;
  Ciphertext out0 = (first + t) << num_dense;
  Ciphertext out1 = (second * drop_front) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```