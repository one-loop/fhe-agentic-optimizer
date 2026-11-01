Attempt 1 made no real change: it has the same operation count as the baseline (one ct-pt multiply, one rotation, one rescale), and the 0.5 ms gap is measurement noise. I don't see a structural improvement left.

- **Rotation is mandatory.** Output slot i must hold input slot i+offset, and a rotation by any nonzero amount costs the same key-switch (~20 ms). One is the minimum.
- **Mask multiply is mandatory.** Dropping it would drop the window masking. Rotating the plaintext mask would need plaintext-side work that isn't free, and the mask is a runtime input, so it can't be pre-rotated at compile time.
- **Order is already optimal.** Rotating after the multiply means the rotation runs at the reduced level. Rotating first would put it at the top level, which is more expensive.

So I'm returning the baseline kernel as the best candidate.

```cpp
// Best known form: one ct-pt multiply by the mask, then one rotation by slots()/4.
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