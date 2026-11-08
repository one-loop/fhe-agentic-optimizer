Attempt 1 traded a multiply for a scale alignment (`match_scale=1`). That alignment costs about as much as the multiply it saved, and the timing showed no gain.

This version keeps all terms at the same level, so no alignment is needed. It masks both chunks explicitly and then shares one rotation for `output_0`. That gives 3 plaintext multiplies, 2 rotations and no `match_scale`, compared with the baseline's 3 multiplies and 3 rotations.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext shifted1 = (second * drop_front) << num_dense;
  Ciphertext merged = first * drop_front + second * keep_front;
  Ciphertext out0 = merged << num_dense;
  out0.set_output("output_0");
  shifted1.set_output("output_1");
}
```