I still have no circuit that beats attempt 2. Every variant I tried either keeps the same operation counts (2 plaintext multiplications, 2 rotations, 1 scale alignment) or adds a rotation or multiplication that costs more than the alignment it saves. I'm submitting the best kernel again.

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