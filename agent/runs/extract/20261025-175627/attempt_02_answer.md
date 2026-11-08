Idea: rotate first, then multiply by the mask. The mask is a plaintext input, so it cannot be rotated at compile time. But a rotation of the ciphertext at the first level is more expensive than at a lower level, so that variant would be slower, and it is also incorrect without a rotated mask.

Instead, I change the circuit by rotating the input first and multiplying afterwards by the mask that has been rotated in the opposite direction. Plaintext rotation is not available in the DSL, so this does not work.

What I can do is change the circuit shape. The mask zeroes everything outside the window, and the rotation then moves the window to slot 0. Without the mask, the window's wrapped data would remain in the output. The mask is therefore needed, and so are the multiply and the rotation.

A different circuit that is legal: `(input << offset) * (mask << offset)` is invalid, because `mask << offset` is a plaintext operation. So I try the order "rotate, then multiply" with the original mask, which gives a wrong result for the shifted window. It would fail correctness, so I do not submit it.

The one valid change is to remove the rescale cost of the multiply by using the mask in a form that avoids a plaintext encoding step. That is not possible either, because the mask is given as plaintext.

I resubmit the best known form with a small structural variation: the multiply and the rotation are kept, but the output is built as `(input << offset) * mask'`, where `mask'` is built from the encrypted-side values. That also cannot be formed. So I keep the baseline circuit.

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