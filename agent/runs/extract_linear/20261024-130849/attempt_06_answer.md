I haven't found anything faster than the baseline. My best result is 59.23 ms (attempt 3), which is the baseline kernel resubmitted and within noise of the original 59.42 ms. I'm resubmitting the same kernel.

The kernel needs one mask multiply and one rotation, and CHEHAB already does the rotation after the rescale, at a lower level. Rotating the mask instead of the ciphertext was rejected in attempt 2 as plaintext-plaintext evaluation. The only variation between my identical attempts has been run-to-run noise.

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