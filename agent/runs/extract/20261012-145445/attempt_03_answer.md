I'm stopping here and keeping the baseline as the final kernel. Three attempts have now produced the same operation count (one ct-pt multiply, one rotation, one rescale), with times of 29.17, 29.64 and 29.31 ms. The spread is measurement noise, not a real difference.

Under the cost model, this kernel is already minimal:
- **One rotation is required.** Output slot i must hold input slot i+offset, and any nonzero rotation costs one key-switch (~20 ms). Rotations can't be cheaper than one.
- **One mask multiply is required.** It applies the window, and the mask is a runtime input, so it can't be folded into a compile-time constant or pre-rotated.
- **The current order is the cheapest.** Rotating after the multiply runs the rotation at the already-reduced level. Rotating first would put it at the top level.

Using the remaining attempts would only re-measure this same graph. I don't have a candidate that removes a rotation or multiply, so I'm not proposing a further change.

```cpp
// Final kernel: one ct-pt multiply by the mask, then one rotation by slots()/4.
// Both operations are required; the rotation runs after the multiply at the lower level.

void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input * mask) << offset;
  output.set_output("output");
}
```