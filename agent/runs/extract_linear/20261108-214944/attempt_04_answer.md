No correct circuit is cheaper than the baseline, so I'm returning it again and recommend stopping here.

The function is output[i] = input[i + n/4] · mask[i + n/4], with n the slot count. Every implementation needs:

- **One plaintext multiply and its rescale.** The mask values are only known at runtime, so there is no way around multiplying by them.
- **One rotation.** The output slots are shifted against the input slots, and this DSL cannot rotate a plaintext (attempt 2). So the ciphertext has to be rotated.

The baseline has exactly these three operations and nothing else. It also rotates after the rescale, which is the cheaper position.

Every other option is either wrong or more expensive:

- **Rotate the input first, then multiply by the unshifted `mask`.** This computes input[i + n/4] · mask[i], which is the wrong result.
- **Rebuild the shift from smaller rotations, or add masking terms.** This only adds operations.

Resubmitting variants can only produce the identical circuit again or a slower one.

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