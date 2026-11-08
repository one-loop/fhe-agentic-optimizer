The baseline uses 3 masked multiplies and 3 rotations. Two identities cut that down:

- **Fewer rotations.** Rotation is linear, so the two rotated terms of `output_0` can be added first and rotated once.
- **Fewer multiplies.** Since `keep_front = 1 - drop_front`, the term `second * keep_front` equals `second - second * drop_front`. The product `second * drop_front` is already needed for `output_1`, so it can be reused.

The result:
- `output_1 = (second * drop_front) << num_dense`
- `output_0 = (first * drop_front - second * drop_front + second) << num_dense`

This needs 2 plaintext multiplies and 2 rotations instead of 3 and 3. The unmasked `second` sits one level above the products. CHEHAB should align it with a mod switch, which is cheap (~1 ms), but I haven't confirmed that.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext d = second * drop_front;
  Ciphertext shifted1 = d << num_dense;
  Ciphertext merged = (first * drop_front - d) + second;
  Ciphertext out0 = merged << num_dense;
  out0.set_output("output_0");
  shifted1.set_output("output_1");
}
```