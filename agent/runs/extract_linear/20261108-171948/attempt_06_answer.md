I have no cheaper circuit to propose, so I'm not submitting another attempt. The last three repeats of the baseline were all rejected as identical to it. The one deviation I tried, dropping the mask multiply, failed correctness with an error of 0.92.

The baseline needs one plaintext multiply (with its rescale) and one rotation. A rotation-first form would need a plaintext-only rotation, which CHEHAB rejects as plain-plain evaluation. Any other structure I can think of adds rotations or multiplications. The best kernel stays the baseline at 4.75 ms.

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