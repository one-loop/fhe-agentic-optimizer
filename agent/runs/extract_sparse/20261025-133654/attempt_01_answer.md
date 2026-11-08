Let n = num_dense = slots/4. The baseline computes:
- out0 = rot(first*drop, n) + rot(second*keep, n)
- out1 = rot(second*drop, n)

That is 3 plaintext multiplies and 3 rotations.

Rotate before masking. Since rot(x*m, n) = rot(x, n)*rot(m, n), the masks can be applied after a shared rotation. Rotating `second` once gives R2 = second << n, and it serves both outputs, but the masks would need to be rotated, which is plaintext-plaintext work and not allowed.

Instead, combine the two terms of out0 before rotating:
- out0 = (first*drop + second*keep) << n. The sum is a single rotation, so rotations drop from 3 to 2 (one for out0, one for out1).
- Plaintext multiplies stay at 3.

Further reduction: out1 = (second*drop) << n and out0 contains (second*keep) << n. Write second*keep = second - second*drop, so out0 = (first*drop + second - second*drop) << n. That is still 2 plaintext multiplies (first*drop and second*drop), and t = second*drop is reused for out1:
- t = second*drop
- out0 = ((first*drop) - t + second) << n
- out1 = t << n

Cost: 2 plaintext multiplies and 2 rotations. The `second` term sits at a shallower level than the multiplied terms, and CHEHAB aligns it with a cheap operation (a mod switch). Scale matching across levels is handled automatically.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext t = second * drop_front;
  Ciphertext f = first * drop_front;
  Ciphertext out0 = (f - t + second) << num_dense;
  Ciphertext out1 = t << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```