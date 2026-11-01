The previous kernel is already close to the minimum for this formulation. Two rotations are needed because the two outputs are distinct rotated ciphertexts, and two ciphertext-plaintext multiplies are needed for `first*drop` and `second*drop`. The remaining `match_scale` comes from adding the level-0 `second` to the level-1 terms. Replacing it with a third multiply (`second*keep`) would cost about as much as the alignment it removes, so I am keeping the attempt-1 structure unchanged.

```cpp
// Reuse t = second*drop; second*keep = second - t, so only 2 pt-mults and 2 rotations are needed.

void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext t = second * drop_front;
  Ciphertext a = first * drop_front;
  Ciphertext u = a + second - t;
  Ciphertext out0 = u << num_dense;
  Ciphertext out1 = t << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```