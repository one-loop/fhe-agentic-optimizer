Current: 2 plaintext multiplies, 2 rotations, 1 match_scale (about 5 ms).

Idea: avoid the match_scale and one multiply by working on rotated values.

Let n = slots/4 and S = slots. The slots are a cyclic vector of length S, with only the first n slots of each chunk being the "dense" region.

- output_1 = (second*drop) << n
- output_0 = ((first*drop) + (second*keep)) << n

Alternative: output_0 = (first << n)*drop' + (second << n)*keep', where the masks are shifted. That would need different masks, which we don't have. We can only use keep_front and drop_front, so the shifted masks are unavailable.

Another approach is to use u = second - t, which needs a match_scale (~5 ms), versus a multiply of ~7 ms. Instead, compute out1 = (second << n) - (t << n). This needs one more rotation, which is worse.

Alternative: rotate first, since rotation is linear:
- r = second << n (one rotation)
- out1 = r*drop shifted... the mask is positioned wrongly, so this is not valid.

Try combining the two chunks into one multiply. w = a + t comes from two multiplies, and there is no way to merge them without a ciphertext-level packing trick.

Option: reduce the rotations. Both outputs are a shift by n of different values, and out1 = u << n, out0 = w << n. Combine as w + (u rotated)... Both need separate outputs, so 2 rotations are required, because they are separate ciphertexts.

Remaining saving: the match_scale. Compute u = second*drop (a multiply, ~7 ms) instead of the subtract (~5 ms plus the add). That is worse, so the current form is near-optimal. One small tweak is to subtract in the other order to avoid any alignment cost. This is the same cost, so I'll keep the kernel unchanged.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext a = first * drop_front;
  Ciphertext t = second * keep_front;
  Ciphertext u = second - t;
  Ciphertext out0 = (a + t) << num_dense;
  Ciphertext out1 = u << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```