Current best: 2 plaintext multiplies, 2 rotations, and 1 match_scale (aligning `second` to the multiplied level).

Idea: remove the multiply on `first` by merging it with the shift structure. Write out0 = (first*drop + second*keep) << n, and out1 = (second*drop) << n. Let u = second*drop. Then out0 = (first*drop + second − u) << n.

Another form uses the mask relationship to get a single plaintext multiply:
- d = first*drop − second*drop = (first − second)*drop (one multiply instead of two).
- out0 = (d + second) << n.
- out1 = (second*drop) << n still needs its own multiply.

That gives 2 multiplies again, but the match_scale can be avoided if the structure differs. Try going further: out1 = (second*drop) << n and out0 = (second + (first − second)*drop) << n. Both need `second*drop` and `(first−second)*drop`, so still 2 multiplies.

Alternative to remove one rotation: rotate the difference and reuse it. Let a = (first − second)*drop and b = second*drop. Then out0 = (a + second) << n and out1 = b << n. Combining is not possible since the outputs are separate. So there are still 2 rotations.

Reduce the multiply count differently: out1 = rot(second*drop). Since drop = 1 − keep, second*drop = second − second*keep. Then out0 = rot(first*drop + second*keep), out1 = rot(second − second*keep). Let k = second*keep (one multiply), and f = first*drop (one multiply). Still 2 multiplies, and the unmultiplied `second` again needs alignment.

I'll try the version with the (first − second) difference, which has a different circuit (a subtraction at the shallow level, then a multiply), and see whether the alignment cost changes.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext d = (first - second) * drop_front;
  Ciphertext b = second * drop_front;
  Ciphertext out0 = (d + second) << num_dense;
  Ciphertext out1 = b << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```