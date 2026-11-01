Baseline does one plaintext multiply (with rescale) and one rotation. The rotation is applied after the multiply, at a lower level where it is cheaper. The alternative is to rotate the input first and multiply by a rotated mask, but the mask is a plaintext input and cannot be rotated without a ciphertext operand, so that does not work.

The two operations are already minimal for this function. Since `output = rotate(input * mask)`, the output depends on the mask at every slot, so I can't drop the multiply. The one option left is to change the order of operations. Rotating first (at the top level) costs more, and the multiply then costs the same, so the baseline order is already the better one.

I'll try a variant that is structurally different, hoping for a small gain: compute the multiply, then rotate with `>>` by the complementary amount (`slots() - offset`). This is the same rotation in a different form, so CHEHAB will probably normalize it to the same circuit. That makes it a low-risk attempt, though it is unlikely to win.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext prod = input * mask;
  Ciphertext output = prod << offset;
  output.set_output("output");
}
```