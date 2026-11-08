Baseline: 3 plaintext multiplies and 3 rotations.

Let a = first*drop, b = second*drop, c = second*keep.
out0 = (a + c) << n, out1 = b << n.

The rotation is linear, so out0 can be computed as one rotation of (a + c). That gives 2 rotations in total, versus 3 in the baseline, and the multiplies stay at 3.

Further reduction: out1 = (second*drop) << n and out0 = (first*drop + second*keep) << n.

Use a mask-free alternative: second*keep = second - second*drop = second - b, so c = second - b. Then a + c = a + second - b. The multiplies drop to 2 (a and b), and the add count rises, which is cheap. The rotations are 2, with the rotation of b needed for out1 and the rotation of (a - b + second) for out0.

Could we rotate first and avoid the multiplies? Rotating the unmasked values would bring in wrapped slots that still need masking, so the multiplies remain.

Another option: out0 = (a - b + second) << n and out1 = b << n. That is 2 multiplies and 2 rotations. The level mismatch is handled by CHEHAB, since `second` is at a shallower level than a and b, which is fine.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext a = first * drop_front;
  Ciphertext b = second * drop_front;
  Ciphertext s = (a - b) + second;
  Ciphertext out0 = s << num_dense;
  Ciphertext out1 = b << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```