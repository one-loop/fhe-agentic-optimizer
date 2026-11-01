The baseline uses 3 plaintext multiplies and 3 rotations. Let slots be S and d = S/4, with the logical input being first‖second.

out0 = (first*drop) << d + (second*keep) << d
out1 = (second*drop) << d

Rotating `first*drop` by d moves slot i+d to i. The mask drop is 1 on slots ≥ d, so the result lands in slots 0..3d-1. Likewise `second*keep` shifted by d puts second's first d slots at positions S-d..S-1 (cyclic wrap). The sum of the two masked products is then a single rotation of the same masked vector:

out0 = ((first*drop) + (second*keep)) << d

This saves one rotation, since the two terms share the shift. The cost is 3 plaintext multiplies, 2 rotations and 1 add.

Next, can the multiplies be reduced? out1 = (second*drop) << d, and `second*keep` is needed for out0. Since drop = 1 - keep, second*drop = second - second*keep, but constants cannot be used and `second` appears unmasked, so this needs the same count of plaintext multiplies: compute t = second*keep (1 mult), then second - t (a subtraction, no multiply). That gives:

- t = second*keep (1 pt mult)
- u = first*drop (1 pt mult)
- out0 = (u + t) << d (1 rotation)
- out1 = (second - t) << d (1 rotation)

That is 2 plaintext multiplies and 2 rotations. Scale concerns: `second` is at level 0 with the original scale, while t is one level down. CHEHAB aligns the shallower operand with a mod switch or a plaintext multiply, which costs about 1–5 ms, compared with a 7 ms multiply saved, so it is likely still a gain.

Alternatively, out1 could be rotated from (second - t) before the subtraction, but that needs more rotations, so I keep the form above.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext u = first * drop_front;
  Ciphertext t = second * keep_front;
  Ciphertext out0 = (u + t) << num_dense;
  Ciphertext out1 = (second - t) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```