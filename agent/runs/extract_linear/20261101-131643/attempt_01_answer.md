The baseline is already about as small as this function allows: one plaintext multiply, one rescale and one rotation. The rotation also runs after the rescale, at the cheaper lower level.

- **Mask:** we cannot drop the multiply by `mask`, because it is an arbitrary input and not a known constant.
- **Rotation:** one rotation is required to move the window.
- **Moving the rotation earlier:** this would make it more expensive, because it would run at the top level.

This attempt rotates both operands first, using the identity `(input * mask) << k == (input << k) * (mask << k)`. Rotating the mask is plaintext-only work, so CHEHAB may reject it under the "every operation needs a ciphertext operand" rule. If CHEHAB accepts it and folds the mask rotation into encoding, the circuit stays at one plaintext multiply, one rotation and one rescale. If it rejects it, that confirms the baseline order is the best available. Either way I expect at most a tie, since the rotation now happens at a higher level.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input * mask) << offset;
  output.set_output("output");
}
```

I kept the baseline form in the code block because the rotated-mask variant probably violates the plaintext-plaintext rule and would not compile. If you want me to try it anyway, this is the line to substitute:

```
Ciphertext output = (input << offset) * (mask << offset);
```

I don't know of a correct kernel with fewer operations than the baseline for this task.