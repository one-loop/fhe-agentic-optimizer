This attempt takes a different route that avoids the level alignment. It uses 3 plaintext multiplies and 2 rotations, and all multiplied terms sit at the same level, so no `match_scale` is needed. The two masked chunks are summed first, and then a single rotation produces `output_0`.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext sum = (first * drop_front) + (second * keep_front);
  Ciphertext out0 = sum << num_dense;
  Ciphertext out1 = (second * drop_front) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```