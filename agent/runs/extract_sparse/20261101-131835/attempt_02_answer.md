The current best already has the minimum structure I can see: two rotations (two full output ciphertexts) and two plaintext multiplies. That leaves the single `match_scale`, inserted to align the level-0 `second`. This attempt removes it by computing `second * keep_front` directly. Every term in the `output_0` sum then has the same one-multiply history, so no alignment is needed. The circuit becomes 3 ct-pt multiplies, 2 rotations, 1 add and no `match_scale`. The trade is one ~7 ms plaintext multiply against the ~5 ms alignment plus an extra subtraction, so this may come out about even.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext out0 = (first * drop_front + second * keep_front) << num_dense;
  Ciphertext out1 = (second * drop_front) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```