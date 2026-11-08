I don't expect any different circuit to beat the baseline, and the attempt below will most likely be slower or fail to compile. The baseline is already minimal: one mask multiply to zero the slots outside the window, and one rotation to move the window to slot 0. Those are the cheapest operations in their order, at one level, with no alignment.

The only structurally different correct circuit is to rotate first and then mask: `(input << k) * (mask << k)`.

- **Might it be cheaper?** If CHEHAB treats the rotated mask as a plaintext prepared at encode time, the multiply-and-rescale becomes the last operation and the plaintext rotation might cost nothing.
- **Why it is probably slower:** the ciphertext rotation now runs at the top level, with one more prime, so it should cost more than in the baseline.
- **Why it might fail:** rotating a plaintext may be rejected as a plaintext-only operation.

I'm submitting it to settle the question. If it fails or is slower, the baseline should stand.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  // Rotate first, then mask with the correspondingly rotated mask:
  // ((input << k) * (mask << k))[i] = input[i+k] * mask[i+k] = ((input*mask) << k)[i]
  Ciphertext rotated = input << offset;
  Plaintext rmask = mask << offset;
  Ciphertext output = rotated * rmask;
  output.set_output("output");
}
```