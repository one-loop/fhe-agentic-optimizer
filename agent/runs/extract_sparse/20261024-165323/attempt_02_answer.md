Current: 3 pt-mults, 2 rotations. Idea: reduce the pt-mults and rotations further.

Let n = slots/4. The logical input is first‖second, with each chunk of size slots. out0 = rot(first*drop + second*keep, n) and out1 = rot(second*drop, n).

Algebra: first*drop + second*keep = first*drop + second - second*drop. Define s1 = second*drop, so comb = first*drop + second - s1. This still needs 2 pt-mults (first*drop and s1), and s1 is reused for out1. That makes 2 pt-mults instead of 3.

Then out0 = rot(first*drop + second - s1, n) and out1 = rot(s1, n). Still 2 rotations.

The mixed levels are fine: `second` is at level 0 while the products are one level deeper, and CHEHAB aligns them with a mod switch (cheap, ~1 ms) rather than a multiply, so there is no scale mismatch error.

Could the rotations be reduced further? out0 = rot(A, n) with A = first*drop + second - s1, and out1 = rot(s1, n). These are two independent rotations, so I keep both.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext s1 = second * drop_front;
  Ciphertext comb = ((first * drop_front) - s1) + second;
  Ciphertext out0 = comb << num_dense;
  Ciphertext out1 = s1 << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```