Baseline: 3 plaintext multiplies and 3 rotations.

Let n = num_dense. Writing out0 = rot(first*drop + second*keep, n) needs only 1 rotation instead of 2, so the sum is combined before rotating. This gives 3 pt-mults, 2 rotations (one for out0, one for out1) and 1 add.

Further, shifted1 = rot(second*drop, n). Could out1 be rotated without a mask? Second*drop is needed to zero the front n slots, which after the rotation land at the end of the vector. The expected output presumably has zeros or other content there, so I keep the mask to be safe.

Now the out0 identity: rot(first*drop + second*keep, n). Both terms are at the same level, with the same multiplication pattern, so the add is valid. The rotation count drops from 3 to 2, and the operation count is 3 pt-mults, 2 rotations, 3 rescales (possibly fewer after CHEHAB's rewrites).

Could the multiplies be reduced further? Rotating second once and reusing it: let R = rot(second, n). Then rot(second*keep, n) = R*rot(keep, n), but that needs a rotated plaintext, which isn't available. So I keep the version above.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext s1 = second * drop_front;
  Ciphertext comb = (first * drop_front) + (second * keep_front);
  Ciphertext out0 = comb << num_dense;
  Ciphertext out1 = s1 << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```