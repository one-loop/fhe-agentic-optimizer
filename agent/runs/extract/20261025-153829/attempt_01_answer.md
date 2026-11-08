The baseline does a plaintext multiply, a rescale and a rotation. Rotating first and then multiplying by a rotated mask would cost the same, and the mask is an input that I can't rotate at compile time. Rotating the mask is plaintext-plaintext arithmetic, which isn't allowed.

Rotations and multiplications are both required here. The mask has to be applied, and the window has to be moved to slot 0. The only way to cut cost is to do the rotation at a deeper level, which is cheaper. Rotating after the multiply and rescale already does this, so the baseline's order is already the better one.

One alternative is to rotate the input first, then multiply by the mask. This puts the mask in the wrong place unless the window is symmetric, so the result would be incorrect.

I don't see a circuit with fewer operations. I'll submit a variant that is functionally the same, though CHEHAB will probably normalize it to the same circuit and it is unlikely to win.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext masked = input * mask;
  Ciphertext output = masked << offset;
  output.set_output("output");
}
```