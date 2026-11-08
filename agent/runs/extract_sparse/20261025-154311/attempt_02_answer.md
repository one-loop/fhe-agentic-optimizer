Current best: 2 plaintext multiplications, 2 rotations, and 1 match_scale (the `second - t` alignment, which costs a multiply_plain plus rescale).

Idea: remove the match_scale and one plaintext multiplication by computing on the unmasked data before rotating.

- out1 = (second*drop) << n. Computing it directly costs one plaintext multiplication, with no alignment.
- out0 = (first*drop + second*keep) << n.

Since drop = 1 - keep, the first term of out0 can be rewritten as first - first*keep, but that doesn't reduce the count.

Another option is to rotate first, then mask. Rotating by n moves slot i+n to slot i. The mask drop applied before the rotation selects the slots n.. of the source. After rotation, those slots sit in positions 0..(slots-n-1), which is the complement of a mask for the last n slots. Define the mask r = 1 - rot(keep), applied after rotation. This needs a plaintext that I can't build, because there are no constants and no plaintext arithmetic. But rot(drop) isn't available either.

So I'll try something else: out0 + out1 relationships. Let R(x) = x << n.
- out1 = R(second) - R(t)
- out0 = R(first*drop) + R(t)

That is 3 rotations, which is worse.

Alternative: remove the alignment by making the subtraction operands match. Compute out1 = (second*drop) << n directly. That is 3 plaintext multiplications, 2 rotations, and no match_scale. Compared with the best, it swaps a match_scale (~5 ms) for a plaintext multiplication (~7 ms), so it's slightly worse.

A cheaper alternative: out0 = (a + t) << n where a = first*drop and t = second*keep, and out1 = (second*drop) << n. Alternatively, out1 = (second - t) << n could be computed as `second + (-t)`, but that is the same circuit after normalization.

One more option: pack both outputs into a single rotation. Compute u = a + t, and v = second - t. Then form w = u + (v rotated by half the slots)? That would require extra masks, so it isn't feasible.

I'll try the 3-plaintext-multiplication variant with a shared rotation structure, accepting the risk that it comes out roughly equal, since the circuit differs and the timing may vary.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext sd = second * drop_front;
  Ciphertext a = first * drop_front;
  Ciphertext t = second * keep_front;
  Ciphertext out0 = (a + t) << num_dense;
  Ciphertext out1 = sd << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```