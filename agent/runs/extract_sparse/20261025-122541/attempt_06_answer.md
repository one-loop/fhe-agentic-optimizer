I can't find a circuit with fewer operations than attempt 2. Each output needs its own rotation, so 2 rotations is the minimum. Each output also needs a mask on `second`. The `first` mask term is merged by the identity, which leaves 2 plaintext multiplications. Avoiding the one scale alignment would cost an extra rotation (~20 ms) or an extra plaintext multiplication (~7 ms), more than the ~5 ms alignment itself. I'm keeping the best kernel.

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