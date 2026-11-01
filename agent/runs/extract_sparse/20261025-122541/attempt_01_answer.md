Let n = num_dense = slots/4. The baseline computes:
- out0 = (first*drop) << n + (second*keep) << n
- out1 = (second*drop) << n

It uses 3 plaintext multiplications and 3 rotations.

Since the rotation is linear, the two terms of out0 can be combined before rotating:
out0 = (first*drop + second*keep) << n

This needs 3 plaintext multiplications and 2 rotations. The sum has the same multiplication history, so there is no scale mismatch.

Can the plaintext multiplications be reduced further? In the dense case, out1 = (second*drop) << n. Rotating first and masking afterwards doesn't help, because the mask would still be needed.

Another option is to rotate second once and share it. Let R = second << n. Then out1 = R * (drop << n), and (drop << n) is a plaintext rotation, which we can't compute without a plaintext-only operation. The rotated mask of drop would be 1 on slots 0..2n-1 (the cyclic shift of drop by n), but we can't build it from the inputs.

So the best option I can see is the merged version, with 3 plaintext multiplications and 2 rotations.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext out0 = (first * drop_front + second * keep_front) << num_dense;
  Ciphertext out1 = (second * drop_front) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```