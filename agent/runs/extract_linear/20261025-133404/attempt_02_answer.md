The rejection came from `mask << offset`, which rotates a plaintext and is treated as a plain-plain operation. The earlier plan was wrong to rely on it.

The baseline already has the minimum operation count: one plaintext multiply with its rescale, and one rotation. A cheaper circuit would have to remove one of those.

The rotation costs about 20 ms. The multiply costs about 7 ms. The output is the window shifted by slots()/4, so the rotation cannot be dropped. The mask must also be applied, since it is the projection diagonal.

One variant that is structurally different is to rotate first and then multiply by the unrotated mask. That computes a different function, because the mask would be misaligned. It is not valid.

Another option is to reduce the rotation cost by splitting the shift into smaller rotations. Each extra rotation costs about 20 ms, so that is worse.

Rotating at a lower level is cheaper, and the baseline already does that by multiplying first. So I resubmit the baseline form. It is still the best known kernel. This attempt gains nothing, but it is correct and safe.

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