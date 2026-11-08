I have no cheaper circuit left to try. I'm resubmitting the best structure from attempt 1, which uses 2 plaintext multiplies, 2 rotations and one level alignment:
- `out0 = ((first - second)*drop + second) << n`
- `out1 = (second*drop) << n`

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext a = first * drop_front;
  Ciphertext b = second * drop_front;
  Ciphertext s = (a - b) + second;
  Ciphertext out0 = s << num_dense;
  Ciphertext out1 = b << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```