I can't find a cheaper circuit, and I'm not going to spend the remaining attempts on variants that CHEHAB normalizes back to the same circuit. The baseline is already at the minimum of one ct-pt multiply with rescale and one rotation.

The only structural change that would help is rotating the input before the multiply. That needs a pre-rotated mask, which would be a plaintext-plaintext operation that the backend rejects. Assuming the mask is zero outside a window would also be unsafe, since the dense test case may not satisfy it.

The best kernel stays the baseline at 59.04 ms:

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