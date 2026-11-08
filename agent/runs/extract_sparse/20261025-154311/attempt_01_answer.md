The baseline spends 3 plaintext multiplications and 3 rotations. The outputs are:

- output_0 = (first*drop + second*keep) << n
- output_1 = (second*drop) << n

Rotation is linear, so the first output can be combined before rotating: output_0 = (first*drop + second*keep) << n. That needs 2 plaintext multiplications and 1 rotation. output_1 needs 1 plaintext multiplication and 1 rotation. The total is then 3 plaintext multiplications and 2 rotations, which saves one rotation (about 20 ms).

Can the plaintext multiplications be reduced further? Since drop = 1 - keep, second*drop = second - second*keep. So with t = second*keep:
- output_1 = (second - t) << n
- output_0 = (first*drop + t) << n

The first*drop term could also be written as first - first*keep, but that doesn't help. Instead, rotate in a different order:
- A = first*drop, t = second*keep (2 plaintext multiplications)
- output_0 = (A + t) << n (1 rotation)
- output_1 = (second - t) << n (1 rotation)

That is 2 plaintext multiplications and 2 rotations. The subtraction mixes second at a higher level with t at a lower level, which CHEHAB aligns for a small cost of about 1 ms. It is a risk, since the alignment may use a multiply_plain, but it should still be cheaper than a 7 ms plaintext multiplication.

The two rotations can't be merged into one, because they apply to different data.

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
  Ciphertext out0 = (a + t) << num_dense;
  Ciphertext out1 = (second - t) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```