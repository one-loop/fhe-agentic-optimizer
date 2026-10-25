I found no correct circuit cheaper than the baseline, and none of my seven attempts beat it. This is the baseline kernel again, with the same circuit and the same 63.48 ms.

- **Attempt 2:** rotating the mask is rejected, because CHEHAB treats it as plaintext-plaintext evaluation.
- **Attempts 1, 3, 4, 5, 6 and 7:** rewrites of the same computation, including a right-rotation form. All compiled to the identical circuit, so none could be timed or win.
- **Rotating the input first:** it needs a mask rotated by the same offset, which the DSL can't produce, so the output would be wrong.

The ordering that keeps the one multiply and one rotation cheapest is multiply, rescale, then rotate. The rotation then runs at a reduced level, which the baseline already does.

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