The baseline does a plaintext multiply (with rescale) followed by a rotation. Rotating first and multiplying by a rotated mask would not help, because the mask is a plaintext input and I can't rotate it without a plaintext-plaintext operation. The rotation also costs less at a deeper level, so the current order, multiply first (which consumes a level) and rotate after, is already the cheaper one.

The remaining option is to rotate the ciphertext first and then multiply by the mask. That doesn't give the same function, since the mask would need to be rotated too. So the baseline structure stays, and I resubmit it as the best-known form. The rotation after the multiply runs at a lower level and is cheaper.

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