Baseline: 3 plaintext multiplies and 3 rotations.

Let n = slots/4 and S = slots. The logical input is first‖second, with drop_front zeroing the first n slots.

- output_0 = (first*drop) << n + (second*keep) << n
- output_1 = (second*drop) << n

Rewrite using linearity of rotation:
- output_0 = ((first*drop) + (second*keep)) << n. That is 2 multiplies and 1 rotation.
- output_1 = (second*drop) << n, which is 1 multiply and 1 rotation.

That totals 3 multiplies and 2 rotations.

Reduce the multiplies further. Let t = second*keep and u = second*drop, so second = t + u. Since drop = 1 - keep, u = second - t, and this needs no multiply (a subtraction only). Then:
- t = second*keep (1 multiply)
- u = second - t (a subtraction; the levels differ, which CHEHAB aligns, though that alignment costs about 5 ms, so it is slightly cheaper than a multiply)
- a = first*drop (1 multiply)
- output_0 = (a + t) << n
- output_1 = u << n

That gives 2 multiplies and 2 rotations. Alternatively, I could drop first*drop by using the rotation structure: (first << n) has garbage in the top n slots, where the wrapped first-part lands. Slots S-n..S-1 of first<<n hold first[0..n-1], which is the dropped part. So output_0 = (first << n) with the last n slots replaced by (second << n)'s first n slots. This needs a mask anyway, so the multiply count stays the same.

A cheaper variant is a single rotation of a combined value, though output_1 needs its own rotation. Let w = a + t (one multiply on each of first and second). Then output_0 = w << n and output_1 = u << n, so the 2-multiply, 2-rotation form is what I'll use.

The subtraction second - t mixes levels (second at level 0 and t at level 1). CHEHAB aligns them for free or at a small cost, which is acceptable.

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