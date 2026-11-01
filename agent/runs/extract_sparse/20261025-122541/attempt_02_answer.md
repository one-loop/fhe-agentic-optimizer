The mask identity keep_front = 1 - drop_front lets me merge the first output's two masked terms into one multiplication:

first·drop + second·keep = second + (first − second)·drop

So out0 = (second + (first − second)·drop) << n needs only one plaintext multiplication. out1 = (second·drop) << n still needs its own. That gives 2 plaintext multiplications and 2 rotations, down from 3 and 2. The un-multiplied `second` is at a different level from the product, so CHEHAB aligns the two at no scale-mismatch cost.

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