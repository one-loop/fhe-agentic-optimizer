The baseline does a plaintext multiply (with rescale) and then a rotation. Rotating first and multiplying afterwards gives the same result with the mask rotated, but the mask is a plaintext input, so I cannot rotate it without a ciphertext operand. That rules out reordering.

What remains is the cost of the rotation. The rotation after the multiply happens at a deeper level, after the rescale, so it already works on a smaller modulus. That is the cheaper order, so the baseline ordering is already good.

I have no way to reduce operation counts here without changing the semantics. The multiply is needed because the mask is arbitrary, and the rotation is needed to move the window. The one variant worth trying is to rotate first and multiply by the mask afterwards, which would apply the mask at the wrong positions. Since that changes the function, I won't do it.

I'll try one structural variant: express the rotation as `>>` with the negative-equivalent amount, `slots() - offset`. CHEHAB probably normalizes this to the same circuit, so it is unlikely to win, but it is a valid attempt.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext masked = input * mask;
  Ciphertext output = masked >> (static_cast<int>(slots()) - offset);
  output.set_output("output");
}
```