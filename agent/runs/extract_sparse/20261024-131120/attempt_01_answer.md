Idea: the baseline does 3 plaintext multiplies and 3 rotations. Let n = slots/4.

out0 = (first*drop) << n + (second*keep) << n
Since rotation is linear, out0 = (first*drop + second*keep) << n, which gives 1 rotation instead of 2.

out1 = (second*drop) << n. This needs 1 rotation too.

Total: 3 plaintext multiplies and 2 rotations.

Can I cut the multiplies further? Masking before the rotation is needed to avoid wraparound junk. Is the junk actually harmful? Rotating by n, slot i takes slot i+n. For `first << n`, slots 0..3n-1 get first[n..4n-1], which is correct. Slots 3n..4n-1 get first[0..n-1], which is the wrapped part; the expected output there is presumably the second chunk's keep part (that's why the mask is applied). So out0 = (first << n) with the top quarter replaced by (second << n)'s top quarter, i.e. second[0..n-1] at slots 3n..4n-1.

Using a single mask: out0 = A + keep*(B - A) where A = first << n, B = second << n. That needs 2 rotations plus 1 multiply, but the mask would be in the rotated frame, so I need a mask for the slots 3n.. rather than keep_front. The available masks are keep_front (first n slots) and drop_front, so that doesn't fit directly.

Alternative: t = (first - second)*keep_front gives the first-chunk's first quarter minus the second's. Then first*drop + second*keep = first - t, where t = (first - second)*keep. So out0 = (first - t) << n, using 1 multiply and 1 rotation.

Check: first - (first - second)*keep gives second on the keep slots and first elsewhere. ✓.

out1 = (second*drop) << n needs 1 multiply and 1 rotation. Can it be shared? second*drop = second - second*keep, which still needs a multiply. Total is 2 multiplies and 2 rotations.

Also, the multiplications happen at the same level, so the subtraction `first - t` has first at a shallower level, which CHEHAB aligns automatically at a modest cost (~1 ms mod switch, or ~5 ms if it needs a multiply_plain). That's acceptable, and it is still cheaper than the extra multiply.

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