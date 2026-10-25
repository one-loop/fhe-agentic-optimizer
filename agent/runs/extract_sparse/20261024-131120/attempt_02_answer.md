Idea: the current best uses 2 plaintext multiplies, 2 rotations, and 1 match_scale (the alignment of `first` with `t`). I can try to cut costs further.

Rotation is linear, so I can rotate before masking, but the masks would then need to be in the rotated frame, and only keep_front and drop_front are available.

Alternative for out1: second*drop = second - second*keep. Together with t = (first - second)*keep, this lets me write:
- s = second*keep (one multiply)
- out1 = (second - s) << n
- out0 = (first - (first*keep) + s)...

That needs more multiplies, so it is not better.

Another option is to combine the rotations. Let u = first - t (the mixed chunk) and v = second*drop. Both need a shift by n, and the results are two separate outputs, so two rotations are needed. A packing trick would need a multiply anyway.

Remaining waste: the match_scale on `first` (~5 ms). To avoid it, I can compute mixed = first - t where `first` is aligned to t's level. The alternative is out0 = (first << n) - (t << n), which needs an extra rotation, so that is worse.

I could restructure so both multiplies share one operand pattern: t = (first - second)*keep, and v = second*drop. Then u = first - t needs `first` at the lower level. Instead, u = (first*drop) + (second*keep) is 2 multiplies, with no alignment but one more multiply. That is the same cost as alignment, so no gain.

I'll keep the best kernel, since the remaining improvements are marginal, and resubmit it.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext t = (first - second) * keep_front;
  Ciphertext mixed = first - t;
  Ciphertext out0 = mixed << num_dense;
  Ciphertext out1 = (second * drop_front) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```